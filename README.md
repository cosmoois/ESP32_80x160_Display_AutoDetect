# ESP32_80x160_Display_AutoDetect

80x160ドットのディスプレイモジュールにおいて、正規のST7735ドライバでなくGC9106が搭載されている場合でも、LovyanGFXで利用できるようにしました。<BR>

## 背景

AliExpressでST7735ドライバの0.96インチディスプレイを調達した際に、正しく動かないものが出てきました。<BR>

ネットを調べていたら、下記のフォーラムにあるように、ST7735のクローンとしてGC9106が乗っているものが出回っているようです。<BR>

https://forum.arduino.cc/t/cropped-80x160-display-st7735s-tft-espi/899248

LovyanGFXのパラメータ変更だけではうまく表示することができなかったので、Arduino_GFXの初期設定を取り込んで利用しようと考えました。

## 機能

display IDをリードして、ディスプレイドライバを判断し、デモ表示を行います。<BR>

IDのリード方法は、下記を参考にしました。<BR>

https://github.com/prenticedavid/ST7789_kbv/blob/master/examples/ST7789_readreg_diag/ST7789_readreg_diag.ino

対応ディスプレイ：
- ST7735S (0.96inch, 80x160)
- GC9106 (0.96inch, 80x160)

利用環境に合わせて、ディスプレイとの接続ピンの指定（DISP_XXXの定義）を変更してください。<BR>

ソースコードはDOIT ESP32 DEVKITを使用して、写真のように直結した状態を想定しています。<BR>

## 注意

直結して使用する場合は十分な配慮が必要です。<BR>

マイコンボードの回路やGPIOポートの能力を十分確認して、自己責任で使用してください。<BR>
＃ピン配置に影響を受けずに、使用できる組み合わせは少ないと思われます。<BR>

特に、GPIOポートから電力を供給する場合は、ポートの能力だけでなく、ディスプレイの消費電力と電源やCPUの供給量力とを見極めて使用してください。<BR>

![GC9106](image/GC9106.jpg)
![ST7735S](image/ST7735S.jpg)

ST7735の消費電流が10mA未満に対し、GC9106は15mA程度消費するようです。<BR>

![測定値](image/current.jpg)
