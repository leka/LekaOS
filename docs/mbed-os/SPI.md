# SPI

## SPI Clock Frequency

Source: [Mbed ST Wiki / SPI output clock frequency](https://os.mbed.com/teams/ST/wiki/SPI-output-clock-frequency)

Based on the source documentation, we are able to define the SPI clock frequency for the STM32F7 family of MCUs:

- from [spi_api.c](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F7/spi_api.c#L62)

```c++
// [...]

case SPI_6:
    /* SPI_1, SPI_4, SPI_5 and SPI_6. Source CLK is PCKL2 */
    spi_hz = HAL_RCC_GetPCLK2Freq();
    break;
case SPI_2:
case SPI_3:
    /* SPI_2 and SPI_3. Source CLK is PCKL1 */
    spi_hz = HAL_RCC_GetPCLK1Freq();
    break;

// [...]
```

- from [system_clock.c](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F7/TARGET_STM32F769xI/TARGET_DISCO_F769NI/system_clock.c#L251)

```c++
#if ((CLOCK_SOURCE) & USE_PLL_HSI) // <-- we are using USE_PLL_HSI

// [...]

RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           //  54 MHz
RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;           // 108 MHz

// [...]
```

| Clock | PCKL1 | PCKL2 |
|:----------:|:-----------------------------:|:---------------:|
| SPI | SPI_1, SPI_4,<br>SPI_5, SPI_6 | SPI_2,<br>SPI_3 |
| Base Clock | 54 MHz | 108 MHz |
| **Scalar** | **PCKL1** | **PCKL2** |
| 2 | 27 MHz | 54 MHz |
| 4 | 13.5 MHz | 27 MHz |
| 8 | 6.75 MHz | 13.5 MHz |
| 16 | 3.38 MHz | 6.75 MHz |
| 32 | 1.69 MHz | 3.38 MHz |
| 64 | 843.75 kHz | 1.69 MHz |
| 128 | 421.88 kHz | 843.75 kHz |
| 256 | 210.94 kHz | 421.88 kHz |

Now if you want to have a SPI output frequency around 1.5 MHz, you will need to pass a value above 1.69 MHz and below 3.38 MHz when you call the SPI frequency method.

```c++
SPI mydevice(SPI_MOSI, SPI_MISO, SPI_SCK);
mydevice.frequency(1800000); // real value = 1.69 MHz
```
