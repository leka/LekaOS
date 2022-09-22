#!/usr/bin/env python3

import sys
from typing import Dict
import argparse
import re


#
# MARK: - Standards
#


def version_is_semver_compliant(version: str):
    # Source: https://semver.org/
    semver_regex = "^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$"

    matches = re.match(semver_regex, version)
    return matches is not (None)


def version_is_mcuboot_compliant(version: str):
    version_splited = version.split(sep=".")

    if len(version_splited) != 3:
        return False

    try:
        if int(version_splited[0]) > 255:
            return False

        if int(version_splited[1]) > 255:
            return False

        if int(version_splited[2]) > 65535:
            return False

    except BaseException as error:
        print(f"{error}")
        return False

    return True


#
# MARK: - argparse
#


parser = argparse.ArgumentParser(
    description="Check os version compliance with standards used in the project"
)

parser.add_argument(
    "file_version_path", type=str, help="path of file containing version to check"
)

args = parser.parse_args()


#
# MARK: - Functions
#


def getDataFromFile(file_path: str) -> str:
    data: str = ""
    try:
        f = open(file_path, "r")
        data = f.read()
        f.close()
    except BaseException as error:
        print(f"{error}")
        sys.exit(1)

    return data


#
# MARK: - Main
#


def main():
    standards_applied: Dict[str, function[[str], bool]] = {}
    standards_applied["Semantic Versioning 2.0.0"] = version_is_semver_compliant
    standards_applied["MCUboot"] = version_is_mcuboot_compliant

    data_containing_version = getDataFromFile(args.file_version_path)

    all_checks_did_pass: bool = True

    for standard_name, version_is_compliant in standards_applied.items():
        if version_is_compliant(data_containing_version):
            print(f"✅ Version is compliant with {standard_name}.")
        else:
            print(f"❌ Version is not compliant with {standard_name}.")
            all_checks_did_pass = False

    if not all_checks_did_pass:
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
