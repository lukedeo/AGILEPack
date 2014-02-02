Using the AGILEPack API
===========

The API for AGILEPack was designed to be as versatile as possible, while still allowing a power user to tweak the package to their own desires. This tutorial will show a very simple example using a fake CSV dataset. We'll use a dataset ([`D.csv`](https://github.com/lukedeo/AGILEPack/blob/master/generic/D.csv)) that looks like this.

```
"X", "X2", "Y"
0.105702184275515, 1.65761733148966, -0.012620126269064
-0.934567215255002, -13.5335926187866, 0.651449144466462
1.42106748564207, 20.8660939658939, -1.17105149032125
1.41813910052268, 20.8233303191078, -1.13185968976074
-1.09174298879155, -15.8288539962416, -0.141019422812328
-0.896676941136426, -12.9802752670808, 0.847703755485907
-1.64363750784683, -23.8882523724308, 2.28458046863387
0.479913257986828, 7.12227772526112, -0.373281316632109
-0.995153891771525, -14.4183489363014, 0.562680542929479
1.24130698522112, 18.2410244368913, -1.61076745845561
...
```

The variable names are `X`, `X2`, and `Y`. Note that the quotation marks around the variable names are optional, as the parser will remove extraneous quotations out of column names. 


