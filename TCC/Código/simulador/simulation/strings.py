generate_help = "Generate perceptions. Require 2 args: total number of perceptions and percentage of invalid perceptions, in this order. Exemple: --generate 500 80"
reload_help = "Reload or not current agent file. If true, will overwrite agent.txt with base-agent.txt file, removing plans created by the autoplanner."
reasoning_time_help = (
    "Define agent's avarege reasoning time. Default is 1, as proposed in the paper."
)
planning_time_help = (
    "Define agent's autoplanner avarege time. Default is 32, but any value can be used."
)
perceptions_per_cycle_help = (
    "Define number of perceptions received by the agent each cycle."
)
iterations_help = "Number of simulations that will be executed."
