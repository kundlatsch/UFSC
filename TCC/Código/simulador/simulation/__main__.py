from simulation import Simulation
from analyzer import get_mean
from generator.perceptions import PerceptionGenerator

import strings
from time import time
import pandas as pd
from shutil import copyfile
import click
import os

from proggy import BarInfo
from proggy.tty import TTYProgressBar


# Click was used in testing and first simulations, but was replaced with
# multiple function calls. This way, it is possible to run the program once
# for all 16 factors variation, instead of passing the arguments through console
# every time one simulation ends. Uncommenting this decorators and the first line
# from main enable console usage again.

# @click.command()
# @click.option("--generate", "-G", default=0, nargs=2, help=strings.generate_help)
# @click.option(
#     "--reload-agent/--not-reload", "-R/", default=True, help=strings.reload_help
# )
# @click.option("--reasoning-time", default=1.0, help=strings.reasoning_time_help)
# @click.option("--planning-time", default=32.0, help=strings.planning_time_help)
# @click.option(
#     "--perceptions-per-cycle", "-C", default=1, help=strings.perceptions_per_cycle_help
# )
# @click.option("--iterations", "-I", default=1, help=strings.iterations_help)
def run(
    generate,
    reload_agent,
    reasoning_time,
    planning_time,
    perceptions_per_cycle,
    iterations,
):

    start_time = time()
    vtimes = []
    pps = []
    pcs = []
    name_string = factors = f"valid{generate[1]}reasoning{reasoning_time}planning{planning_time}perceptions{perceptions_per_cycle}reload{str(reload_agent)}"

    with TTYProgressBar(BarInfo(size=30, total=iterations)) as p:

        for i in range(iterations):
            # print(f"-------------\nSIMULATION {i}\n-------------")

            if generate:
                g = PerceptionGenerator(*generate, perceptions_per_cycle)
                g.generate()
                if not os.path.exists(f'./data3/perceptions/{name_string}'):
                    os.makedirs(f'./data3/perceptions/{name_string}')
                copyfile("perceptions.txt", f"./data3/perceptions/{name_string}/iteration{i}.txt")

            s = Simulation(
                reasoning_time,
                planning_time,
                perceptions_per_cycle,
                reload_agent=reload_agent,
            )
            vtime, perceptions_processed, plans_created = s.start()
            vtimes.append(vtime)
            pps.append(perceptions_processed)
            pcs.append(plans_created)

            if not os.path.exists(f'./data3/agents/{name_string}'):
                    os.makedirs(f'./data3/agents/{name_string}')
            copyfile("agent.txt", f"./data3/agents/{name_string}/iteration{i}.txt")

            p.progress += 1

    final_time = time()
    total_time = final_time - start_time
    total_time = "{:.2f}".format(total_time)
    print(
        f"------------\nFinished Simulation\nTime elapsed: {total_time}s\n------------"
    )

    d = {"vtime": vtimes, "perceptions_processed": pps, "plans_created": pcs}
    df = pd.DataFrame(data=d)
    
    df.to_csv(f'./data3/results/{name_string}.csv', index=False)


if __name__ == "__main__":
    # run()
    run((5000, 50), True, 16, 32, 8, 1)
    run((5000, 50), False, 16, 32, 8, 1)
    run((5000, 50), False, 16, 32, 8, 1)
    run((5000, 50), False, 16, 32, 8, 1)
    run((5000, 50), False, 16, 32, 8, 1)
    # run((5000, 5), False, 1, 0.5, 1, 1)
    # run((5000, 5), True, 1, 0.5, 16, 1)
    # run((5000, 5), False, 1, 0.5, 16, 1)
    # run((5000, 5), True, 1, 64, 1, 1)
    # run((5000, 5), False, 1, 64, 1, 1)
    # run((5000, 5), True, 1, 64, 16, 1)
    # run((5000, 5), False, 1, 64, 16, 1)
    # run((5000, 5), True, 32, 0.5, 1, 1)
    # run((5000, 5), False, 32, 0.5, 1, 1)
    # run((5000, 5), True, 32, 0.5, 16, 1)
    # run((5000, 5), False, 32, 0.5, 16, 1)
    # run((5000, 5), True, 32, 64, 1, 1)
        
    
    
    
