#!/usr/bin/env ruby

# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

FIRMWARE_STATISTICS_OUTPUT_PATH="#{ENV["RUNNER_HOME"]}/FIRMWARE_STATISTICS_OUTPUT.md"
STATUS_DIFF_OUTPUT_PATH="#{ENV["RUNNER_HOME"]}/STATUS_DIFF_OUTPUT.md"
MAP_DIFF_OUTPUT_PATH="#{ENV["RUNNER_HOME"]}/MAP_DIFF_OUTPUT.md"

FIRMWARE_STATISTICS_OUTPUT=File.read(FIRMWARE_STATISTICS_OUTPUT_PATH)
STATUS_DIFF_OUTPUT=File.read(STATUS_DIFF_OUTPUT_PATH)
MAP_DIFF_OUTPUT=File.read(MAP_DIFF_OUTPUT_PATH)

sticky_message = <<-EOF

# :chart_with_upwards_trend: Changes Impact Analysis Report

## :pushpin: Info

- toolchain: `#{ENV["ARM_GNU_TOOLCHAIN_VERSION"]}`
- enable_log_debug: `#{ENV["ENABLE_LOG_DEBUG"]}`

|           | `base`                                                                           | `head`                                                                           |
|-----------|----------------------------------------------------------------------------------|----------------------------------------------------------------------------------|
| branch    | [`#{ENV["BASE_REF"]}`](https://github.com/leka/LekaOS/tree/#{ENV["BASE_REF"]})   | [`#{ENV["HEAD_REF"]}`](https://github.com/leka/LekaOS/tree/#{ENV["HEAD_REF"]})   |
| sha       | #{ENV["BASE_SHA"]}                                                               | #{ENV["HEAD_SHA"]}                                                               |
| mbed      | `#{ENV["BASE_MBED_VERSION"]}`                                                    | `#{ENV["HEAD_MBED_VERSION"]}`                                                    |
| `-std`    | `#{ENV["BASE_CXX_STANDARD"]}`                                                    | `#{ENV["HEAD_CXX_STANDARD"]}`                                                    |

## :robot: Firmware impact analysis

#{FIRMWARE_STATISTICS_OUTPUT}

<details>
<summary>Click to show memory sections</summary>

```
| -          |      Hex |     Bytes |  KiB |
|------------|---------:|----------:|-----:|
| Flash      | 0x200000 | 2 097 152 | 2048 |
| SRAM       |  0x80000 |   524 288 |  512 |
| Bootloader |  0x40000 |   262 144 |  256 |
| Header     |   0x1000 |     4 096 |    4 |
| OS         | 0x17E000 | 1 564 672 | 1528 |
| Tail       |   0x1000 |     4 096 |    4 |
| Scratch    |  0x40000 |   262 144 |  256 |
```

</details>

## :microscope: Detailed impact analysis

<details>
<summary>Click to show detailed analysis for all targets</summary>

- :heavy_check_mark: - existing target
- :sparkles: - new target
- :coffin: - deleted target
- :white_check_mark: - files are the same
- :x: - files are different

#{STATUS_DIFF_OUTPUT}

## :world_map: Map files diff output

#{MAP_DIFF_OUTPUT}

</details>

EOF

puts sticky_message

STICKY_MESSAGE_PATH = "#{ENV["RUNNER_HOME"]}/STICKY_MESSAGE.md"

File.write(STICKY_MESSAGE_PATH, "#{sticky_message}", mode: 'a')
