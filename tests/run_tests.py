import subprocess
import pathlib
import pytest

# Path to the executable
LSYS_EXE = pathlib.Path(__file__).parent.parent / "build" / "lsystems"

def collect_test_cases(folder, expect_success=True):
    input_dir = pathlib.Path(__file__).parent / folder
    test_cases = []
    for ls_file in input_dir.glob("*.lsy"):
        expected_file = ls_file.with_suffix(".expected")
        test_cases.append((ls_file, expected_file, expect_success))
    return test_cases

@pytest.mark.parametrize("ls_file, expected_file, expect_success", collect_test_cases("success", expect_success=True))
def test_success_cases(ls_file, expected_file, expect_success):
    result = subprocess.run(
        [str(LSYS_EXE), str(pathlib.Path(ls_file))],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=2
    )
    output = result.stdout.decode("utf-8")
    expected = expected_file.read_text(encoding="utf-8")

    assert result.returncode == 0, f"{ls_file.name} failed unexpectedly"
    assert output == expected, f"{ls_file.name} output mismatch"

@pytest.mark.parametrize("ls_file, expected_file, expect_success", collect_test_cases("failure", expect_success=False))
def test_failure_cases(ls_file, expected_file, expect_success):
    result = subprocess.run(
        [str(LSYS_EXE), str(pathlib.Path(ls_file))],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=2
    )
    output = result.stdout.decode("utf-8") + result.stderr.decode("utf-8")
    expected = expected_file.read_text(encoding="utf-8")

    assert result.returncode != 0, f"{ls_file.name} should fail but succeeded"
    assert expected in output, f"{ls_file.name} error message mismatch"
