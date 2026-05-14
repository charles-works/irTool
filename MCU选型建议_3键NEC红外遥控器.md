> Legacy/background note: this document describes the earlier STM8L001J3M3 option. The active default for the current repository is STM8L051F3P6 + IRLML2502; see `docs/design/stm8l051f3p6-irlml2502-migration.md`.

# 3键 NEC 红外遥控器 MCU 选型建议

## 1. 项目背景

本项目需要设计一个 3 键 NEC 红外遥控器，发射码要求如下：

| 按键 | 用户码 | 用户反码 | 按键码 | 按键反码 | 完整 NEC 帧 |
|---|---:|---:|---:|---:|---|
| KEY1 | `80` | `7F` | `C0` | `3F` | `80 7F C0 3F` |
| KEY2 | `80` | `7F` | `40` | `BF` | `80 7F 40 BF` |
| KEY3 | `80` | `7F` | `60` | `9F` | `80 7F 60 9F` |

红外协议为标准 NEC 协议：

```text
Leader + 用户原码 + 用户反码 + 按键原码 + 按键反码 + Stop bit
```

由于 µPD6121 / µPD6122 的按键数据码由内部固定键矩阵决定，不能自由指定为 `0xC0 / 0x40 / 0x60`，因此推荐使用 MCU 直接产生 NEC 红外波形。

---

## 2. 已有开发和烧录工具

当前已有工具：

| 工具 | 可用于 | 说明 |
|---|---|---|
| ST-LINK | STM8 / STM32 | 支持 SWIM / SWD 烧录调试 |
| PWLINK2 | ARM Cortex-M / STM8 等 | 适合多种 MCU 在线烧录调试，具体支持列表需以软件为准 |
| USB 转 TTL | 串口 ISP / 串口日志 | 适合 STC、部分带 UART Bootloader 的 MCU |

因此 MCU 选型应优先考虑：

1. 支持现有工具直接烧录；
2. 电路外围简单；
3. 成本低；
4. 低功耗，适合电池供电；
5. 可方便产生 38 kHz 红外载波；
6. 开发调试难度低。

---

## 3. 红外遥控器对 MCU 的最低资源需求

### 3.1 GPIO 需求

至少需要：

```text
3 个按键输入 GPIO
1 个红外输出 GPIO / PWM 输出
1 个烧录调试接口
```

推荐额外保留：

```text
1 个串口 TX，用于调试日志，可选
1 个测试点，用于示波器观察时序，可选
```

### 3.2 定时器需求

MCU 需要能产生约 `38 kHz` 载波。

NEC 常用载波参数：

```text
频率：38 kHz
周期：约 26.3 us
占空比：约 1/3
高电平时间：约 8.7 us
低电平时间：约 17.6 us
```

推荐使用定时器 PWM 产生载波：

```text
发送载波时：开启 PWM 输出
发送空闲时：关闭 PWM 输出
```

这样比纯软件翻转更稳定，也更省 CPU。

### 3.3 低功耗需求

遥控器绝大多数时间处于待机状态，因此 MCU 应支持：

- 睡眠 / Stop / Halt 模式；
- GPIO 按键唤醒；
- 内部 RC 振荡器启动；
- 无需外部晶振；
- 待机电流尽量低于数 µA。

---

## 4. MCU 候选型号对比

## 4.1 推荐排序总表

| 推荐级别 | MCU | 架构 | 烧录/调试 | 优点 | 注意点 |
|---|---|---|---|---|---|
| 首推量产 | `STM8L001J3M3` | STM8 | ST-LINK / SWIM | 低功耗、小封装、成本低、自带 IR 相关资源 | SO-8 引脚紧张 |
| 首推开发 | `STM32C011F4P6` | Cortex-M0+ | ST-LINK / SWD | 调试方便、生态好、资源充足 | 成本略高于 8 位 MCU |
| 低成本国产 | `PY32F002A` | Cortex-M0+ | SWD / PWLINK2 视支持情况 | 成本低、电压范围宽、资源够用 | 需确认烧录器支持 |
| 串口 ISP | `STC8G1K08A` | 1T 8051 | USB-TTL / UART ISP | 成本低、烧录简单、外围少 | 在线调试体验较弱 |

---

# 5. 方案一：STM8L001J3M3

## 5.1 推荐程度

```text
推荐级别：★★★★★
适合阶段：最终产品 / 小体积低功耗遥控器
```

## 5.2 主要特点

`STM8L001J3M3` 是低功耗 STM8L 系列 MCU，适合电池供电小产品。

主要优点：

- SO-8 小封装；
- 工作电压适合 3 V 电池系统；
- 低功耗能力好；
- 支持 SWIM 烧录调试；
- ST-LINK 可直接使用；
- 定时器资源足够产生 38 kHz PWM；
- Flash 容量足够实现 NEC 编码；
- 成本较低。

## 5.3 推荐电路结构

```text
3V 电池
  |
  +-- STM8L001J3M3
        |
        +-- GPIO_KEY1 ---- 按键 ---- GND
        +-- GPIO_KEY2 ---- 按键 ---- GND
        +-- GPIO_KEY3 ---- 按键 ---- GND
        |
        +-- TIM_PWM_OUT ---- NPN/NMOS ---- 红外 LED
        |
        +-- SWIM 烧录调试口
        +-- NRST
        +-- VDD / GND
```

## 5.4 烧录接口建议

建议在 PCB 上预留 4 个测试点：

```text
VDD
GND
SWIM
NRST
```

连接 ST-LINK：

```text
ST-LINK VCC  -> 目标板 VDD
ST-LINK GND  -> 目标板 GND
ST-LINK SWIM -> MCU SWIM
ST-LINK NRST -> MCU NRST
```

## 5.5 优点总结

```text
低功耗、小体积、成本低、现有 ST-LINK 可烧录，适合最终产品。
```

## 5.6 注意事项

- SO-8 引脚较少，调试阶段要规划好 SWIM 引脚；
- 建议不要省略 NRST 测试点；
- 按键建议使用内部上拉，外部按键接地；
- 红外输出建议用定时器 PWM，不建议长期依赖软件延时翻转。

---

# 6. 方案二：STM32C011F4P6

## 6.1 推荐程度

```text
推荐级别：★★★★☆
适合阶段：样机开发 / 需要频繁调试 / 代码后续扩展
```

## 6.2 主要特点

`STM32C011F4P6` 属于 STM32C0 系列，Cortex-M0+ 内核。

主要优点：

- 支持 SWD 在线调试；
- 可直接使用 ST-LINK；
- STM32CubeIDE / STM32CubeProgrammer 支持好；
- 定时器资源充足，产生 38 kHz PWM 很方便；
- GPIO 较多，方便调试和扩展；
- 可保留 UART 调试日志；
- 代码迁移和维护方便。

## 6.3 推荐电路结构

```text
3V 电池
  |
  +-- STM32C011F4P6
        |
        +-- PAx ---- KEY1 ---- GND
        +-- PAx ---- KEY2 ---- GND
        +-- PAx ---- KEY3 ---- GND
        |
        +-- TIMx_CHx ---- NPN/NMOS ---- 红外 LED
        |
        +-- USART_TX ---- USB-TTL RX，可选
        |
        +-- SWDIO
        +-- SWCLK
        +-- NRST
        +-- VDD / GND
```

## 6.4 SWD 烧录调试接口

建议预留 5 个测试点或 2.54 mm 排针：

```text
VDD
GND
SWDIO
SWCLK
NRST
```

连接 ST-LINK：

```text
ST-LINK 3.3V/VREF -> 目标板 VDD
ST-LINK GND       -> 目标板 GND
ST-LINK SWDIO     -> MCU SWDIO
ST-LINK SWCLK     -> MCU SWCLK
ST-LINK NRST      -> MCU NRST
```

## 6.5 优点总结

```text
开发和调试最方便，适合先做样机验证。
```

## 6.6 注意事项

- 成本通常高于 STM8L001 / STC / 部分国产低端 MCU；
- 若用于最终量产遥控器，资源会有一定富余；
- 低功耗需要正确配置 Stop / Standby 模式和 GPIO 状态。

---

# 7. 方案三：PY32F002A

## 7.1 推荐程度

```text
推荐级别：★★★★☆
适合阶段：低成本国产化方案
```

## 7.2 主要特点

`PY32F002A` 是低成本 Cortex-M0+ MCU，适合小型控制应用。

主要优点：

- Cortex-M0+ 内核；
- 成本低；
- 工作电压范围较宽；
- 支持 SWD；
- 内部 RC 可用，无需外部晶振；
- 定时器可产生 38 kHz PWM；
- 有 SOP8 / SOP16 等小封装可选。

## 7.3 推荐电路结构

```text
3V 电池
  |
  +-- PY32F002A
        |
        +-- GPIO_KEY1 ---- 按键 ---- GND
        +-- GPIO_KEY2 ---- 按键 ---- GND
        +-- GPIO_KEY3 ---- 按键 ---- GND
        |
        +-- PWM_OUT ---- NPN/NMOS ---- 红外 LED
        |
        +-- SWDIO
        +-- SWCLK
        +-- NRST，可选
        +-- VDD / GND
```

## 7.4 烧录调试建议

如果使用 PWLINK2，需要先确认软件支持列表中是否包含具体型号：

```text
PY32F002A
PY32F002B
PY32F030
```

若支持，则可通过 SWD 方式烧录：

```text
PWLINK2 VREF -> 目标板 VDD
PWLINK2 GND  -> 目标板 GND
PWLINK2 SWDIO -> MCU SWDIO
PWLINK2 SWCLK -> MCU SWCLK
PWLINK2 NRST  -> MCU NRST，可选
```

## 7.5 优点总结

```text
成本低，资源够用，适合对成本敏感的最终产品。
```

## 7.6 注意事项

- 必须确认现有 PWLINK2 / 开发软件支持该具体型号；
- 相比 STM32，资料和生态可能略弱；
- 批量采购前建议先做小样验证烧录、低功耗和红外时序稳定性。

---

# 8. 方案四：STC8G1K08A

## 8.1 推荐程度

```text
推荐级别：★★★☆☆
适合阶段：极低成本 / 只想使用 USB-TTL 烧录
```

## 8.2 主要特点

`STC8G1K08A` 是 STC 1T 8051 系列 MCU，常用于低成本小控制器。

主要优点：

- 成本低；
- 可用 USB 转 TTL 直接 ISP 烧录；
- 外围简单；
- 内部 RC，无需外部晶振；
- 有定时器 / PWM 资源；
- 适合简单红外遥控器。

## 8.3 推荐电路结构

```text
3V 电池
  |
  +-- STC8G1K08A
        |
        +-- GPIO_KEY1 ---- 按键 ---- GND
        +-- GPIO_KEY2 ---- 按键 ---- GND
        +-- GPIO_KEY3 ---- 按键 ---- GND
        |
        +-- PWM/GPIO_OUT ---- NPN/NMOS ---- 红外 LED
        |
        +-- RXD / TXD ---- USB-TTL
        +-- VDD / GND
```

## 8.4 烧录方式

使用 USB 转 TTL：

```text
USB-TTL TXD -> MCU RXD
USB-TTL RXD -> MCU TXD
USB-TTL GND -> MCU GND
```

烧录时一般需要配合：

```text
断电再上电
或使用自动下载电路控制电源/复位
```

## 8.5 优点总结

```text
工具要求最低，USB-TTL 即可烧录，成本低。
```

## 8.6 注意事项

- 在线调试能力不如 SWD / SWIM；
- 软件开发生态不如 STM32；
- 需要注意 STC ISP 上电时序；
- 若使用 C51 开发，需要适应 8051 编程方式。

---

# 9. 推荐最终选型

## 9.1 如果优先考虑样机开发效率

推荐：

```text
STM32C011F4P6
```

理由：

- ST-LINK 直接 SWD 调试；
- CubeIDE 生态完善；
- 定时器配置方便；
- GPIO 多，方便接逻辑分析仪、串口日志和调试引脚；
- 出问题时排查效率最高。

适合：

```text
第一版样机 / 协议验证 / 波形调试
```

## 9.2 如果优先考虑最终产品成本、功耗和体积

推荐：

```text
STM8L001J3M3
```

理由：

- SO-8 小封装；
- 低功耗表现好；
- 成本较低；
- ST-LINK 可烧录；
- 资源刚好满足 3 键 NEC 遥控器。

适合：

```text
最终产品 / 电池供电 / 小型遥控器
```

## 9.3 如果优先考虑国产低成本

推荐：

```text
PY32F002A
```

前提：

```text
确认 PWLINK2 或现有烧录器支持该型号。
```

适合：

```text
成本敏感 / 国产化 / 小批量量产
```

## 9.4 如果只想用 USB-TTL 完成烧录

推荐：

```text
STC8G1K08A
```

适合：

```text
不使用 ST-LINK / 不需要在线调试 / 极简低成本
```

---

# 10. 推荐硬件通用结构

无论选择哪种 MCU，红外遥控器的总体电路都可以保持如下结构。

## 10.1 电源

```text
2 × AAA 电池
或
1 × CR2032 纽扣电池
```

建议：

```text
MCU VDD-GND：0.1 µF 去耦电容
电池输入：4.7 µF ~ 47 µF 储能电容
```

如果使用 CR2032，建议 LED 峰值电流不要过大。

## 10.2 按键

推荐接法：

```text
MCU GPIO ---- 按键 ---- GND
```

MCU 内部配置：

```text
上拉输入
下降沿唤醒
低电平表示按下
```

优点：

- 外部元件少；
- 待机功耗低；
- 易于布线。

## 10.3 红外 LED 驱动

推荐 NPN 或 NMOS 低边驱动。

### NPN 示例

```text
VCC 3V
 |
 +---- 红外 LED ---- 限流电阻 ---- NPN 集电极
                                      |
                                  NPN 发射极
                                      |
                                     GND

MCU_PWM ---- 1kΩ ---- NPN 基极
NPN 基极 ---- 100kΩ ---- GND，可选
```

### 限流电阻估算

假设：

```text
VCC = 3.0 V
红外 LED VF = 1.2 V
NPN 饱和压降 = 0.2 V
目标峰值电流 = 50 mA
```

计算：

```text
R = (3.0 - 1.2 - 0.2) / 0.05 ≈ 32 Ω
```

推荐取值：

| 限流电阻 | 适用场景 |
|---:|---|
| `33 Ω` | AAA 电池，发射距离较远 |
| `47 Ω` | 通用折中 |
| `68 Ω` | CR2032，降低电流 |

---

# 11. 软件实现建议

## 11.1 NEC 码值定义

```c
#define NEC_USER_CODE      0x80
#define NEC_USER_CODE_INV  0x7F

#define KEY1_CODE          0xC0
#define KEY2_CODE          0x40
#define KEY3_CODE          0x60
```

## 11.2 NEC 完整帧发送

```c
void nec_send_frame(uint8_t cmd)
{
    uint8_t inv = ~cmd;

    send_carrier(9000);
    delay_us(4500);

    send_byte_lsb(NEC_USER_CODE);
    send_byte_lsb(NEC_USER_CODE_INV);
    send_byte_lsb(cmd);
    send_byte_lsb(inv);

    send_carrier(560);
}
```

## 11.3 NEC 字节发送

```c
void send_byte_lsb(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) {
        send_carrier(560);

        if (data & 0x01) {
            delay_us(1690);
        } else {
            delay_us(560);
        }

        data >>= 1;
    }
}
```

## 11.4 长按 repeat

标准 NEC repeat：

```text
9 ms 载波 + 2.25 ms 空闲 + 560 us 载波
```

伪代码：

```c
void nec_send_repeat(void)
{
    send_carrier(9000);
    delay_us(2250);
    send_carrier(560);
}
```

推荐逻辑：

```text
首次按下：发送完整帧
持续按下：每约 108 ms 发送 repeat 帧
松开：停止
```

如果接收端不识别 repeat，也可以每 108 ms 重发完整帧。

---

# 12. PCB 和调试建议

## 12.1 必留测试点

建议无论选择哪款 MCU，都预留：

```text
VDD
GND
红外输出测试点
```

若为 SWD MCU，预留：

```text
SWDIO
SWCLK
NRST
```

若为 STM8，预留：

```text
SWIM
NRST
```

若为 STC，预留：

```text
RXD
TXD
GND
VDD
```

## 12.2 调试流程

1. 先不装红外 LED，测 MCU PWM 输出是否为 38 kHz；
2. 装红外 LED 后，用示波器或逻辑分析仪查看接收头输出；
3. 验证 KEY1 / KEY2 / KEY3 解码结果；
4. 测量待机电流；
5. 测量按键发射时峰值电流；
6. 测试不同距离和角度下的接收可靠性。

---

# 13. 最终建议

综合考虑你已有：

```text
ST-LINK
PWLINK2
USB 转 TTL
```

以及项目要求：

```text
电路简单、成本低、低功耗、方便烧录调试
```

推荐路线如下：

## 阶段 1：快速验证

```text
STM32C011F4P6
```

用于快速调通 NEC 波形、按键逻辑、长按 repeat、红外发射距离。

## 阶段 2：产品化

优先选择：

```text
STM8L001J3M3
```

用于低功耗、小体积、低成本产品板。

## 阶段 3：进一步降成本

如果 PWLINK2 支持且供货稳定，可考虑：

```text
PY32F002A
```

## 不需要在线调试时

如果只追求 USB-TTL 烧录和极低成本，可考虑：

```text
STC8G1K08A
```

---

# 14. 推荐结论

最终推荐优先级：

```text
1. STM8L001J3M3：最终产品首推
2. STM32C011F4P6：样机开发首推
3. PY32F002A：低成本国产替代
4. STC8G1K08A：USB-TTL 极简方案
```

如果只选一个型号作为本项目最终方案，推荐：

```text
STM8L001J3M3
```

如果先做调试样机，推荐：

```text
STM32C011F4P6
```
