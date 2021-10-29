"""Perception Revision System implementation.

This module implements all the formal model described in the
illusion and hallucination paper. It can be attached to any
cognitive architecture to reason about the perceptions coming
from the environment.
"""

from utils import (
    parse_agent_plans,
    get_perceptions_actions,
    get_agent_context,
    parse_perception,
)

from structures import AvaliationBlock, Autoplanner

from typing import List, Tuple
from random import choice


class PerceptionRevision:
    def __init__(self, agent, reasoning_at, autoplanning_at):
        self.agent = agent
        self.plans = parse_agent_plans(self.agent)
        self.actions = get_perceptions_actions(self.plans)

        # Now, the context only includes terms from the left side of the plan.
        # Verify if the model uses this definition!
        self.context_bodies, self.context_args = get_agent_context(self.plans)
        self.context = self.context_bodies + self.context_args

        self.illusion1_AB = AvaliationBlock(reasoning_at, autoplanning_at)
        self.illusion2_AB = AvaliationBlock(reasoning_at, autoplanning_at)
        self.hallucination_AB = AvaliationBlock(reasoning_at, autoplanning_at)
        self.reasoning_at = reasoning_at

        # This could be replaced with a set, but random.choice
        # gives the error "'set' object is not subscriptable"
        self.avaliation_blocks = []

        self.MAP_PERCEPTION_TO_AB = {
            "illusion1": self.illusion1_AB,
            "illusion2": self.illusion2_AB,
            "hallucination": self.hallucination_AB,
        }

        self.autoplanner = Autoplanner(self.actions, self.context, agent)

    def __update_context(self):
        self.plans = parse_agent_plans(self.agent)
        self.context_bodies, self.context_args = get_agent_context(self.plans)
        self.context = self.context_bodies + self.context_args

    def __classify_perception(self, perception: str) -> str:
        """Classify if a perception is an illusion or hallucination.

        Args:
            perception: The perception string in the format p(x).
        Returns:
            An int, 1 for illusion class 1, 2 for illusion class 2 and 3 for hallucination.
        """
        body, arg = parse_perception(perception)

        if body in self.context and arg in self.context:
            return "valid"

        if body in self.context:
            return "illusion1"

        if arg in self.context:
            return "illusion2"

        return "hallucination"

    def process_perceptions(self, perceptions: List[int]) -> Tuple[int, int]:
        # Return = (vtime, perceptions_processed)

        have_anomaly = False
        anomalies = 0
        # Add each perception to it's respective avaliation block
        for perception in perceptions:
            perception_type = self.__classify_perception(perception)

            if perception_type in self.MAP_PERCEPTION_TO_AB:

                ab = self.MAP_PERCEPTION_TO_AB[perception_type]
                ab.list.push(perception)

                if ab not in self.avaliation_blocks:
                    self.avaliation_blocks.append(ab)

                have_anomaly = True
                anomalies = anomalies + 1

        if not have_anomaly:
            return (self.reasoning_at, len(perceptions))

        vtime = 0
        keep_planning = True

        while keep_planning:
            if self.avaliation_blocks:
                avaliation_block = choice(self.avaliation_blocks)
                (vtime, keep_planning) = avaliation_block.evaluate(vtime)

                if keep_planning:
                    plan_perception = avaliation_block.list.pop()
                    self.autoplanner.plan(plan_perception)

                if avaliation_block.list.is_empty():
                    self.avaliation_blocks.remove(avaliation_block)
            else:
                keep_planning = False

        self.__update_context()

        return (vtime, len(perceptions) - anomalies)
