#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import os
import sys


def addToEnvironement(data: str):
    env_file = os.getenv("GITHUB_ENV")
    with open(env_file, "a") as env:
        env.write(data + "\n")


def getDataFromFile(file_path: str, mode="r"):
    try:
        f = open(file_path, mode)
        data = f.read()
        f.close()
        return data
    except BaseException as error:
        print(f"{error}")
        sys.exit(1)
