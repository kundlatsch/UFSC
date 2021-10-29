from pr_system import PerceptionRevision
from shutil import copyfile


class Simulation:
    def __init__(
        self,
        reasoning_at,
        autoplanning_at,
        perceptions_per_cycle,
        reload_agent=True,
        debug=False,
    ):
        self.perception_queue = []
        self.perceptions_per_cycle = perceptions_per_cycle
        
        perceptions = open("perceptions.txt", "r")

        content = perceptions.readlines()
        for c in content:
            splited = c.split(",")
            for s in splited:
                self.perception_queue.append(s.replace("\n", ""))

        perceptions.close()

        # A copy of the base agent is created because the autoplanner
        # will change the file.
        if reload_agent:
            copyfile("base-agent.txt", "agent.txt")

        self.model = PerceptionRevision("agent.txt", reasoning_at, autoplanning_at)
        self.debug = debug

    def start(self):
        vtime = 0
        perceptions_processed = 0

        perceptions_number = self.perceptions_per_cycle

        while self.perception_queue:
            if perceptions_number > len(self.perception_queue):
                perceptions_number = len(self.perception_queue)

            perceptions = [
                self.perception_queue.pop(0) for i in range(perceptions_number)
            ]

            (_vtime, pp) = self.model.process_perceptions(perceptions)
            vtime = vtime + _vtime
            perceptions_processed = perceptions_processed + pp

        if self.debug:
            print(f"vtime: {vtime}\nperceptions_processed: {perceptions_processed}")
            print(f"{self.model.autoplanner.plans_created} new plans created")

        return (vtime, perceptions_processed, self.model.autoplanner.plans_created)
