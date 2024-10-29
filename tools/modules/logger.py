# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0


"""
Configure logging settings based on verbosity level.
"""


import logging
import sys


def configure_logging(verbose: bool):
    """
    Configure logging settings based on verbosity level.

    Args:
        verbose (bool): If True, set logging level to DEBUG; otherwise, INFO.
    """
    logging.basicConfig(
        level=logging.DEBUG if verbose else logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        handlers=[logging.StreamHandler(sys.stdout)],
    )
    if verbose:
        logging.debug("Verbose logging enabled.")
