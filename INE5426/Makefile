
SHELL := /bin/bash

# Colors
GREEN=\033[0;32m
RED=\033[0;31m
NC=\033[0m

filepath=examples/exemplo1.ccc

help:
	@echo Available commands:
	@req:		Generate requirements.txt from pyproject.toml, for delivering


req:
	@echo -e "${GREEN} Generating requirements.txt... ${NC}"
	@poetry export -f requirements.txt > requirements.txt

setup:
	@echo Installing virtualenv using pip3
	@pip3 install virtualenv --user
	@echo -e "${RED} Removing previous existing virtualenv (if exists)... ${NC}"
	@rm -rf venv/
	@echo -e "${GREEN} Creating new virtualenv...${NC}\n"
	@python3 -m venv venv/
	@source venv/bin/activate && pip install -r requirements.txt
	@echo -e "${GREEN} Virtualenv created! ${NC}"

run:
	@echo -e "${GREEN} Executing... ${NC}"
	@./venv/bin/python src/run.py ${filepath}

poetry-run:
	@echo -e "${GREEN} Executing... ${NC}"
	@poetry run python src/run.py ${filepath}

