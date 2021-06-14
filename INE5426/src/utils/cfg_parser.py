from typing import Set, Union, List

from utils.data_structures import Cfg, Production


class CfgParser():
    def __init__(self):
        self.__current_symbol: Union[None, str] = None

        self.__empty = '&'

        self.__productions: List[Production] = []
        self.__start_symbol: Union[None, str] = None
        self.__non_terminals: Set[str] = set()
        self.__terminals: Set[str] = set()

    def parse(self, filepath: str) -> Cfg:
        with open(filepath) as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue

                self.__parse_line(line)

        return Cfg(start_symbol=self.__start_symbol,
                   terminals=self.__terminals,
                   non_terminals=self.__non_terminals,
                   productions=self.__productions)

    def __parse_line(self, line: str) -> None:
        """Parse a single line"""
        if self.__is_first_production_line(line):
            head, body = self.__split_head_body(line)
            head = head.strip()
            body = body.strip()

            self.__current_symbol = head

            if self.__start_symbol is None:
                self.__start_symbol = head

            self.__parse_production(head, body)

        else:
            self.__parse_production(self.__current_symbol, line.split('|')[-1])

    def __split_head_body(self, prod: str) -> str:
        return prod.split(':')

    def __is_first_production_line(self, line: str) -> bool:
        return len(line.split(':')) == 2

    def __parse_production(self, head: str, str_body: str) -> None:
        body: List[str] = []
        self.__non_terminals.add(head)
        for item in str_body.split():
            item = item.strip()
            if item == '':
                continue

            if item == self.__empty:
                body.append(item)

            elif (item[0] == '"' and item[-1] == '"'):
                nt_symbol = item[1:-1]
                body.append(nt_symbol)

                self.__terminals.add(nt_symbol)

            else:
                self.__non_terminals.add(item)
                body.append(item)

        self.__productions.append(Production(head, body))
