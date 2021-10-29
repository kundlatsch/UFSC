from .perceptions_options import (
    VALID_POSSIBLE_PERCEPTIONS_BODIES,
    VALID_POSSIBLE_PERCEPTIONS_ARGS,
    INVALID,
)

from random_words import RandomWords
from random import choice, shuffle
from time import time


class PerceptionGenerator:
    def __init__(self, cycles, invalid_perceptions_percentage, perceptions_per_line):
        self.cycles = cycles
        self.invalid_p = invalid_perceptions_percentage
        self.valid_p = 100 - invalid_perceptions_percentage
        self.perceptions_per_line = perceptions_per_line

    def create_valid_perception(self):
        body = choice(VALID_POSSIBLE_PERCEPTIONS_BODIES)
        arg = choice(VALID_POSSIBLE_PERCEPTIONS_ARGS)
        return f"{body}({arg})"

    def generate(self):
        start_time = time()

        valid = []
        invalid = []

        valid_cycles = int(self.valid_p * self.cycles / 100)
        invalid_cycles = int(self.invalid_p * self.cycles / 100)

        random_words = RandomWords()
        random_bodies = []
        random_args = []

        # This module only allows requesting 5450 words at once.
        # In some cases, we need 76000 words, so we need to do it "manually"
        for i in range(self.perceptions_per_line):
            bodies = random_words.random_words(count=invalid_cycles)
            args = random_words.random_words(count=invalid_cycles)
            random_bodies = random_bodies + bodies
            random_args = random_args + args

        for i in range(valid_cycles):
            # Generate perceptions for line
            perception_line = []
            for j in range(self.perceptions_per_line):

                perception = self.create_valid_perception()
                while perception in INVALID:
                    perception = self.create_valid_perception()

                perception_line.append(perception)

            # Concatenate perceptions to create line
            final_perception = ""
            for perception in perception_line:
                final_perception = final_perception + perception + ","

            # Add final perception string without the last char
            valid.append(final_perception[:-1])

        for i in range(invalid_cycles):
            # Generate perceptions for line
            perception_line = []
            for j in range(self.perceptions_per_line):
                body = random_bodies.pop(0).lower()
                arg = random_args.pop(0).lower()

                perception = f"{body}({arg})"
                perception_line.append(perception)

            # Concatenate perceptions to create line
            final_perception = ""
            for perception in perception_line:
                final_perception = final_perception + perception + ","

            # Add final perception string without the last char
            invalid.append(final_perception[:-1])

        perceptions = valid + invalid
        shuffle(perceptions)

        file = open("perceptions.txt", "w")

        for line in perceptions:
            file.write(line)
            file.write("\n")

        file.close()

        final_time = time() - start_time
        # print(f"{len(perceptions)} perceptions cycles generated in {final_time}s")
