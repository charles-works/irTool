# 3键 NEC 红外遥控器方案设计分析

## 1. 需求说明

基于 `UPD6121(1).PDF` 文档，设计一个 3 键红外遥控器，协议要求如下：

- 红外协议：NEC 协议
- 用户码：`0x807F`
  - 用户原码：`0x80`
  - 用户反码：`0x7F`
- 3 个按键码：

| 按键 | 按键原码 | 按键反码 | 完整 NEC 码 |
|---|---:|---:|---|
| KEY1 | `0xC0` | `0x3F` | `80 7F C0 3F` |
| KEY2 | `0x40` | `0xBF` | `80 7F 40 BF` |
| KEY3 | `0x60` | `0x9F` | `80 7F 60 9F` |

要求所有发射信号均为：

```text
用户原码 + 用户反码 + 按键原码 + 按键反码
```

---

## 2. µPD6121 / µPD6122 文档关键内容分析

`µPD6121 / µPD6122` 是 NEC 格式红外遥控发射专用 CMOS IC。

### 2.1 芯片主要特性

根据文档：

- 工作电压：`2.0 V ~ 3.3 V`
- 推荐典型电压：`3 V`
- 待机电流：最大约 `1 µA`
- 振荡频率：`400 kHz ~ 500 kHz`
- 推荐陶瓷谐振器：`455 kHz`
- 红外载波频率：

```text
fc = fosc / 12
```

当：

```text
fosc = 455 kHz
```

则：

```text
fc ≈ 455 kHz / 12 ≈ 38 kHz
```

即标准 NEC 红外载波频率。

---

## 3. NEC 发射格式分析

### 3.1 完整帧结构

µPD6121 / µPD6122 的 NEC 发送格式为：

```text
Leader Code + Custom Code + Custom Code' + Data Code + Data Code' + Stop Bit
```

其中：

```text
Custom Code    = 用户码原码，8 bit
Custom Code'   = 用户码反码，8 bit
Data Code      = 数据码原码，8 bit
Data Code'     = 数据码反码，8 bit
```

### 3.2 时间参数

当 `fosc = 455 kHz` 时，文档给出的 NEC 时序如下：

| 项目 | 时间 |
|---|---:|
| Leader 载波 | `9 ms` |
| Leader 空闲 | `4.5 ms` |
| bit 载波宽度 | `0.56 ms` |
| 逻辑 0 总宽度 | `1.125 ms` 左右 |
| 逻辑 1 总宽度 | `2.25 ms` 左右 |
| Repeat Leader 载波 | `9 ms` |
| Repeat 空闲 | `2.25 ms` |
| 重复周期 | 约 `108 ms` |

标准表达为：

```text
逻辑 0：560 us 载波 + 560 us 空闲
逻辑 1：560 us 载波 + 1690 us 空闲
```

### 3.3 位发送顺序

文档明确说明：

```text
Codes are transmitted from the LSB.
```

即：

```text
所有字节均低位先发，LSB first。
```

---

## 4. 目标码值分析

### 4.1 用户码

需求给定：

```text
用户码 = 0x807F
```

拆分为 NEC 格式：

```text
Custom Code  = 0x80
Custom Code' = 0x7F
```

二进制表示：

```text
0x80 = 1000 0000
0x7F = 0111 1111
```

二者互为反码，符合 NEC 常规 8 bit 地址 + 8 bit 地址反码格式。

### 4.2 按键码

#### KEY1

```text
Data Code  = 0xC0
Data Code' = ~0xC0 = 0x3F
完整帧：80 7F C0 3F
```

#### KEY2

```text
Data Code  = 0x40
Data Code' = ~0x40 = 0xBF
完整帧：80 7F 40 BF
```

#### KEY3

```text
Data Code  = 0x60
Data Code' = ~0x60 = 0x9F
完整帧：80 7F 60 9F
```

---

## 5. µPD6121 / µPD6122 是否适合本需求

### 5.1 用户码配置能力

µPD6121 / µPD6122 的用户码通过外部二极管和上拉电阻设置。

对于 Ver I 规格型号：

```text
µPD6121G-001
µPD6122G-001
```

文档说明：

- Custom Code 高 8 bit 由 CCS 与 KI/O 引脚之间是否连接二极管决定；
- Custom Code' 低 8 bit 由 KI/O 引脚上的上拉电阻决定是否取反或不取反；
- 理论上可配置 65,536 种用户码。

因此目标用户码：

```text
0x807F
```

从用户码角度看，使用 Ver I 版本理论上可以实现。

### 5.2 数据码配置限制

µPD6121 / µPD6122 的数据码不是任意可编程的 8 bit 数据，而是由固定键矩阵产生。

#### µPD6121

- KI 输入：`KI0 ~ KI3`
- KI/O 输出：`KI/O0 ~ KI/O7`
- 单键数量：`4 × 8 = 32`
- 数据码范围由键矩阵固定生成

#### µPD6122

- KI 输入：`KI0 ~ KI7`
- KI/O 输出：`KI/O0 ~ KI/O7`
- 单键数量：`8 × 8 = 64`
- 数据码同样由键矩阵固定生成

### 5.3 与目标数据码的冲突

目标按键码为：

```text
0xC0 = 1100 0000b
0x40 = 0100 0000b
0x60 = 0110 0000b
```

若按 NEC 常规 D7~D0 分析：

| 数据码 | D7 | D6 | D5 | D4~D0 |
|---|---:|---:|---:|---:|
| `0xC0` | 1 | 1 | 0 | 0 |
| `0x40` | 0 | 1 | 0 | 0 |
| `0x60` | 0 | 1 | 1 | 0 |

根据文档：

- D7 由 SEL 引脚决定；
- SEL 接 VDD 时，D7 = 0；
- SEL 接 VSS 时，D7 = 1；
- SEL 是全局配置，不是每个按键独立配置；
- 单键模式下 D5 通常无法任意设置为 1；
- D5 = 1 主要出现在特定双键组合模式中。

因此存在以下问题：

1. `0xC0` 需要 D7 = 1，而 `0x40`、`0x60` 需要 D7 = 0；  
   但 SEL 是全局状态，无法同时满足。
2. `0x60` 需要 D5 = 1；  
   普通单键矩阵不能直接输出此码。
3. 双键方式虽能产生部分 D5 = 1 的码，但不适合普通 3 键独立遥控器，且无法得到指定的 `0x60`。

### 5.4 结论

```text
µPD6121 / µPD6122 不适合直接实现本需求。
```

原因是：

```text
用户码可配，但按键数据码不可任意配置，无法可靠直接产生 0xC0 / 0x40 / 0x60 三个指定数据码。
```

---

## 6. 推荐总体方案

推荐使用：

```text
低功耗 MCU + 38 kHz 红外调制 + 红外 LED 驱动电路
```

由 MCU 软件直接生成 NEC 协议波形。

### 6.1 方案优势

- 可完全指定用户码和按键码；
- 支持 NEC 原码 + 反码；
- 可灵活控制长按 repeat 行为；
- 便于后期增加按键或修改码值；
- 硬件简单，成本低；
- 不受 µPD6121 / µPD6122 固定键码表限制。

---

## 7. 硬件设计方案

### 7.1 系统框图

```text
+----------------+
|  电池 3V        |
|  CR2032 / AAA   |
+--------+-------+
         |
         v
+----------------+
|  低功耗 MCU     |
|                |
|  GPIO_KEY1      |<--- KEY1
|  GPIO_KEY2      |<--- KEY2
|  GPIO_KEY3      |<--- KEY3
|                |
|  PWM/GPIO_OUT   |---> 红外发射驱动
+----------------+
         |
         v
+----------------+
|  NPN / NMOS     |
|  红外 LED       |
+----------------+
```

### 7.2 MCU 选型建议

可选 MCU：

| 类型 | 示例 |
|---|---|
| AVR | ATtiny13A、ATtiny25、ATtiny85 |
| PIC | PIC12F、PIC16F 小封装型号 |
| STM8 | STM8S003、STM8L 系列 |
| 国产低功耗 MCU | Padauk、NY8、SC92F、CMS 等 |

最低资源需求：

- 3 个按键输入 GPIO；
- 1 个红外输出 GPIO 或 PWM 输出；
- 低功耗睡眠能力；
- 定时器可产生约 38 kHz 载波；
- Flash 容量很小即可满足。

### 7.3 按键电路

推荐按键接法：

```text
MCU GPIO ---- 按键 ---- GND
```

GPIO 配置：

```text
内部上拉输入
低电平表示按键按下
```

优点：

- 外围器件少；
- 静态功耗低；
- 可用 GPIO 中断唤醒 MCU。

### 7.4 红外 LED 驱动电路

推荐使用 NPN 三极管或小信号 NMOS 驱动红外 LED。

#### NPN 驱动示例

```text
VCC 3V
 |
 |---- 红外 LED ---- 限流电阻 ---- NPN 集电极
                                  NPN 发射极 ---- GND

MCU_IR_OUT ---- 1kΩ ---- NPN 基极
NPN 基极 ---- 100kΩ ---- GND，可选
```

#### 限流电阻估算

假设：

```text
VCC = 3.0 V
红外 LED VF ≈ 1.2 V
NPN VCE(sat) ≈ 0.2 V
目标峰值电流 I = 50 mA
```

则：

```text
R = (3.0 - 1.2 - 0.2) / 0.05 ≈ 32 Ω
```

可选值：

| 电阻 | 说明 |
|---:|---|
| `33 Ω` | 发射功率较强，适合 AAA 电池 |
| `47 Ω` | 通用折中选择 |
| `68 Ω` | 电流较小，适合 CR2032 |

### 7.5 电源与去耦

建议：

```text
MCU VDD 与 GND 之间放置 0.1 µF 陶瓷电容
电池输入处放置 4.7 µF ~ 47 µF 电容
```

若使用 CR2032，建议限制 LED 峰值电流，避免电池压降过大。

---

## 8. 软件设计方案

### 8.1 码值定义

```c
#define NEC_USER_CODE      0x80
#define NEC_USER_CODE_INV  0x7F

#define KEY1_CODE          0xC0
#define KEY2_CODE          0x40
#define KEY3_CODE          0x60
```

### 8.2 NEC 发送函数逻辑

```c
void nec_send_frame(uint8_t cmd)
{
    uint8_t cmd_inv = ~cmd;

    send_carrier(9000);     // 9 ms leader carrier
    delay_us(4500);         // 4.5 ms space

    send_byte_lsb(NEC_USER_CODE);
    send_byte_lsb(NEC_USER_CODE_INV);
    send_byte_lsb(cmd);
    send_byte_lsb(cmd_inv);

    send_carrier(560);      // stop bit
}
```

### 8.3 字节发送

```c
void send_byte_lsb(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) {
        send_carrier(560);

        if (data & 0x01) {
            delay_us(1690);     // logic 1
        } else {
            delay_us(560);      // logic 0
        }

        data >>= 1;
    }
}
```

### 8.4 38 kHz 载波生成

若使用 GPIO 软件翻转：

```text
38 kHz 周期 ≈ 26.3 us
1/3 占空比：高电平约 8.7 us，低电平约 17.6 us
```

伪代码：

```c
void send_carrier(uint16_t time_us)
{
    uint16_t cycles = time_us / 26;

    while (cycles--) {
        IR_OUT_HIGH();
        delay_us(9);
        IR_OUT_LOW();
        delay_us(17);
    }
}
```

更推荐使用 MCU 定时器 PWM 产生 38 kHz：

```text
发送载波时：打开 PWM 输出
空闲时：关闭 PWM 输出，并保持红外输出无效
```

### 8.5 主循环

```c
int main(void)
{
    system_init();
    gpio_init();
    timer_pwm_38k_init();

    while (1) {
        if (key1_pressed()) {
            nec_send_frame(KEY1_CODE);
            wait_key_release_or_repeat(KEY1_CODE);
        }
        else if (key2_pressed()) {
            nec_send_frame(KEY2_CODE);
            wait_key_release_or_repeat(KEY2_CODE);
        }
        else if (key3_pressed()) {
            nec_send_frame(KEY3_CODE);
            wait_key_release_or_repeat(KEY3_CODE);
        }
        else {
            enter_sleep();
        }
    }
}
```

---

## 9. 长按与 Repeat 设计

NEC 标准长按 repeat 码为：

```text
9 ms 载波 + 2.25 ms 空闲 + 560 us 载波
```

重复周期约：

```text
108 ms
```

### 9.1 标准 NEC Repeat 方式

```text
首次按下：发送完整帧
持续按下：每约 108 ms 发送 repeat 帧
松开：停止发送
```

repeat 发送函数：

```c
void nec_send_repeat(void)
{
    send_carrier(9000);
    delay_us(2250);
    send_carrier(560);
}
```

### 9.2 完整帧重复方式

某些接收端不识别 NEC repeat，只识别完整码。此时可采用：

```text
按住按键时，每约 108 ms 重发一次完整帧
```

两种方式对比：

| 方式 | 优点 | 缺点 |
|---|---|---|
| 标准 repeat | 省电，符合 NEC 常规 | 接收端需支持 repeat 解码 |
| 完整帧重复 | 接收端兼容性强 | 平均电流更大 |

建议实际项目根据接收端要求选择。

---

## 10. 最终发射码表

| 按键 | 用户码 | 用户反码 | 数据码 | 数据反码 | 完整帧 |
|---|---:|---:|---:|---:|---|
| KEY1 | `80` | `7F` | `C0` | `3F` | `80 7F C0 3F` |
| KEY2 | `80` | `7F` | `40` | `BF` | `80 7F 40 BF` |
| KEY3 | `80` | `7F` | `60` | `9F` | `80 7F 60 9F` |

---

## 11. 测试验证建议

### 11.1 示波器验证

在红外 LED 驱动端或 MCU 输出端测试：

- 载波频率是否约为 `38 kHz`；
- Leader 是否为 `9 ms + 4.5 ms`；
- bit 0 是否为 `560 us + 560 us`；
- bit 1 是否为 `560 us + 1690 us`；
- 每个字节是否低位先发。

### 11.2 红外接收头验证

使用常见 38 kHz 红外接收头，例如：

```text
VS1838B / HS0038 / TL1838
```

接到逻辑分析仪或 MCU，解码结果应为：

```text
KEY1：80 7F C0 3F
KEY2：80 7F 40 BF
KEY3：80 7F 60 9F
```

### 11.3 手机摄像头粗测

可用手机摄像头观察红外 LED 是否发光，仅用于粗略确认红外 LED 工作，不能验证协议正确性。

---

## 12. 结论

1. `µPD6121 / µPD6122` 是 NEC 红外遥控专用发射芯片，适合固定键码表应用。
2. 其用户码可通过外部二极管和电阻配置，Ver I 理论上可实现 `0x807F`。
3. 但其数据码由固定键矩阵决定，不能任意设置为 `0xC0 / 0x40 / 0x60`。
4. 因此本项目不推荐直接使用 `µPD6121 / µPD6122`。
5. 推荐采用低功耗 MCU 直接生成 NEC 协议，可准确实现：

```text
KEY1：80 7F C0 3F
KEY2：80 7F 40 BF
KEY3：80 7F 60 9F
```

6. 推荐硬件架构为：

```text
3V 电池 + 低功耗 MCU + 3 个按键 + NPN/NMOS 红外 LED 驱动
```

该方案实现灵活、成本低、协议完全可控，适合作为本 3 键 NEC 红外遥控器的最终设计方案。
