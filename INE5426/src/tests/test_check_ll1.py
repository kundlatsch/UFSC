import pytest

from utils.cfg_processor import CfgProcessor


@pytest.fixture
def example_file() -> str:
    return 'tests/example.csf'


@pytest.fixture
def ll1_example_file() -> str:
    return 'tests/ll1_example.csf'


@pytest.fixture
def correct_firsts() -> dict:
    return {
        'S': {'a', 'b', 'c', 'd'},
        'A': {'a', '&'},
        'B': {'a', 'b', 'd', '&'},
        'a': {'a'},
        'b': {'b'},
        'c': {'c'},
        'd': {'d'}
    }


@pytest.fixture
def correct_follows() -> dict:
    return {
        'S': {'$'},
        'A': {'a', 'b', 'c', 'd'},
        'B': {'c'}
    }


@pytest.fixture()
def cfg_proc(example_file: str) -> CfgProcessor:
    cfg_proc = CfgProcessor()
    cfg_proc.read(example_file)

    return cfg_proc


@pytest.fixture()
def cfg_proc_ll1(ll1_example_file: str) -> CfgProcessor:
    cfg_proc = CfgProcessor()
    cfg_proc.read(ll1_example_file)

    return cfg_proc


def test_proc_first(cfg_proc: CfgProcessor, correct_firsts: dict):
    for key, value in correct_firsts.items():
        print('Checking for non-terminal', key)
        assert cfg_proc.first(key) == value  # nosec


def test_proc_follow(cfg_proc: CfgProcessor, correct_follows: dict):
    for key, value in correct_follows.items():
        print('Checking for non-terminal', key)
        assert cfg_proc.follow(key) == value  # nosec


def test_is_ll1(cfg_proc_ll1: CfgProcessor):
    assert cfg_proc_ll1.is_ll1()  # nosec


def test_is_not_ll1(cfg_proc: CfgProcessor):
    assert not cfg_proc.is_ll1()  # nosec
