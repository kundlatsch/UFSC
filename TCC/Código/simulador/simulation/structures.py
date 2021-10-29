from typing import Tuple, TypeVar
from random import choice, randint

T = TypeVar("T")


class OrderedList:
    def __init__(self):
        self.__list = {}

    def pop(self) -> T:
        """Get the value that was inserted more times"""
        first = True
        top = None
        for element in self.__list:
            if first:
                top = element
                first = False

            elif self.__list[element] > self.__list[element]:
                top = element

        self.__list.pop(top, None)
        return top

    def push(self, element: T):
        """Insert an element on the ordered list.

        If it's the first time that the element is inserted,
        it's value is 1. If it's not, it's value will be increased
        by 1.
        
        Args:
            element: The element to be inserted in the list.
        """
        if element in self.__list:
            old_value = self.__list[element]
            self.__list[element] = old_value + 1
        else:
            self.__list[element] = 1

    def clean(self):
        """Clean list to free memory.
        
        Let S1 be the sum of 'weights' (number of times an element was inserted)
        of the elements that the weight is bigger than 1.
        Let S2 be the sum of 'weights' of the elements that hava weight 1.
        If S1 is bigger than S2, remove all the elements in the list that have
        'weight' 1.
        """
        S1 = 0
        S2 = 0
        for element in self.__list:
            weight = self.__list[element]
            if weight > 1:
                S1 = S1 + weight
            if weight == 1:
                S2 = S2 + weight

        if S1 > S2:
            new_list = {}

            for element in self.__list:
                if self.__list[element] > 1:
                    new_list[element] = self.__list[element]

            self.__list = new_list

    def is_empty(self):
        return not bool(self.__list)


class AvaliationBlock:
    """Trivial implementation of the Avaliation Block.

    In this case, the Avaliation Block is just the ordered list. In other
    scenarios, the process of choosing which perception to treat could be
    more complex, or the information from the Ordered List could be passed
    to other elements in the model or in the agent architecture.
    """

    # at = average time
    def __init__(self, reasoning_at: int, autoplanning_at: int):
        self.list = OrderedList()
        self.reasoning_at = reasoning_at
        self.autoplanning_at = autoplanning_at

    def evaluate(self, vtime: int) -> Tuple[int, bool]:
        """Define if a perception in the Ordered List can be processed by the autoplanner."""
        if vtime < self.reasoning_at:
            vtime = vtime + self.autoplanning_at
            return (vtime, True)
        else:
            return (vtime, False)


class Autoplanner:
    """Naïve autoplanning implementation.

    In this implementation, we just randomly pick agent's action and
    combine with the perceptions choosed to create a new plan.
    """

    def __init__(self, actions, context, agent):
        self.actions = actions
        self.context = context
        self.agent = agent
        self.plans_created = 0

    def plan(self, perception):

        actions_number = randint(1, len(self.actions))

        actions = []

        for i in range(actions_number):
            action = choice(self.actions)
            if action not in actions:
                actions.append(action)

        new_actions = []

        for action in actions:
            arg = choice(self.context)
            new_actions.append(f"{action}({arg})")

        new_plan = f"{perception} ->"

        for action in new_actions:
            new_plan = new_plan + " " + action + ";"

        agent_file = open(self.agent, "a")

        agent_file.write("\n" + new_plan)

        agent_file.close()

        self.plans_created = self.plans_created + 1
