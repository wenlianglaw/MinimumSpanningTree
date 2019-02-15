# MinimumSpanningTree
MSP implementation and testing

Implementation of O(V^2) and O(V logE)

Benchmark on 2.2 GHz Intel Core i7, 32 GB 2400 MHz DDR4

| .     | Vertexs Count | Edges Count | Execution Time |
|-------|---------------|-------------|----------------|
|**O(V logE)**|20000|1989849(0.5%)|**439ms**|
|O(V^2)|||1079ms|
|||||
|O(V logE)|10000|497391(0.5%)|92ms|
|O(V^2)|||251ms|
|||||
|**O(V logE)**|10000|989875(1%)|**185ms**|
|O(V^2)|||266ms|
|||||
|**O(V logE)**|20000|3959954(1%)|**893ms**|
|O(V^2)|||1131ms|
|||||
|O(V logE)|5000|1189220(5%)|237ms|
|**O(V^2)**|||**87ms**|
