from pathlib import Path
import subprocess
import pytest
import os


BLACKBOX_DIR = Path(__file__).parent / "blackbox"
EXE = os.environ.get("BF_EXE", None) # type: str # pyright: ignore[reportAssignmentType]
if EXE is None:
    if os.name == "nt":
        # Windows
        EXE = "./build/cbf.exe"
    else:
        # Linux/macOS
        EXE = "./build/cbf"


@pytest.mark.parametrize(
    "case",
    sorted(BLACKBOX_DIR.glob("*.bf")),
    ids=lambda case: case.name,
)
def test_blackbox_passing(case):
    expected_file = case.with_suffix(".expected")
    stdin_file = case.with_suffix(".stdin")
    stdin = stdin_file.read_text() if stdin_file.exists() else None

    assert expected_file.exists(), f"Missing expected output: {expected_file}"

    result = subprocess.run(
        [EXE, str(case)],
        capture_output=True,
        text=True,
        input=stdin,
    )

    assert result.returncode == 0, (
        f"{case} failed with exit code {result.returncode}\n"
        f"stderr:\n{result.stderr}"
    )

    assert result.stdout == expected_file.read_text(), (
        f"Wrong output for {case}"
    )

def test_blackbox_not_closed_loop():
    case = Path(__file__).parent / "blackbox" / "errors" / "error_loop_not_closed.bf"

    result = subprocess.run(
        [EXE, str(case)],
        capture_output=True,
        text=True,
    )

    assert result.returncode == 254, (
        f"{case} failed with exit code {result.returncode}\n"
        f"expected exit code 254/(-2)"
    )

def test_blackbox_not_opened_loop():
    case = Path(__file__).parent / "blackbox" / "errors" / "error_loop_not_opened.bf"

    result = subprocess.run(
        [EXE, str(case)],
        capture_output=True,
        text=True,
    )

    assert result.returncode == 253, (
        f"{case} failed with exit code {result.returncode}\n"
        f"expected exit code 253/(-3)"
    )
