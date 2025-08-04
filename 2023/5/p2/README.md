It does the job. 

but not well. 

```powershell
PS C:\Users\Dwight\Desktop\c_dev\AoC\2023\5\p2> Measure-Command { .\main.exe }

Days              : 0
Hours             : 0
Minutes           : 1
Seconds           : 33
Milliseconds      : 767
Ticks             : 937673178
TotalDays         : 0.00108526988194444
TotalHours        : 0.0260464771666667
TotalMinutes      : 1.56278863
TotalSeconds      : 93.7673178
TotalMilliseconds : 93767.3178
```

1 minute and 34 seconds. 
that is quite bad. 

Interesting. 

One can consider the problem backwards aswell. mapping location -> seed. 
This way we can map until we find a seed that is viable.
