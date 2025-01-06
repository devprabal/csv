## Hashtable format

```c
sched_hash[zone][day][sched_no]
```

| sched_hash array index | max limit |
|-|-|
| zone | 3 |
| day | 7 |
| sched_no | 10 |

## csv format

```txt
day, zone, st, et, workedFlag, enabledFlag, actuatorDetail
```

| field | format | valid values `[]` | not valid values `[]` |
| - | - | - | - |
| day | ddd | [ Mon, Tue, Sat, Sun, Wed, Thu ] | [ mon, MON, T, Th, fri ]
| zone | regex format `^z[123]{1}$` | [z1, z2, z3] | [zone1, zone2, zone3, z12, z23, dhw, z34, z4, z5 ] |
| st | HH:MM (UTC, 24h) | [ 10:33, 04:51, 22:44, 00:12 ] | [ 4:12, 6:15, 23:1, 24:01, 0:02, 23:60, 04:67 ] |
| et | HH:MM (UTC, 24h) | [ 10:33, 04:51, 22:44, 00:12 ] | [ 4:12, 6:15, 23:1, 24:01, 0:02, 23:60, 04:67 ] |
| workedFlag | 0 or 1 | [ 0, 1 ] | [ -1, -2.3, 0.4, 00, 01, 1.23, 3, 5.0 ] |
| enabledFlag | 0 or 1 | [ 0, 1 ] | [ -1, -2.3, 0.4, 00, 01, 1.23, 3, 5.0 ] | 
| actuatorDetail | 0xMMTTTTPP | [ 0x42022F0F, 0x320078F0 ] | [ 0x42022R0F, 0xAB0000A0 ] |  

---

`actuatorDetail` format `0xMMTTTTPP` explanation   

| field | representaion | bytes | example | meaning |
| - | - | - | - | - |
| MM | mode hex value | 1 (unsigned) | 42 | 42 might correspond to 'Heat' mode |
| TTTT | temp hex value | 2 (signed) | 022F | when converted to signed `int16_t` it represents 559 and when divided by 10, it represents the actual temperature 55.9 in decimal |
| PP | power hex value | 1 (unsigned) | F0 | F0 might map to power OFF |

## Generate some sample data using py script

```bash
devp@IdeaPad:~/ehs_sched$ python3 gen_csv_data.py
```


```csv

tue, z3, 05:43, 12:58, 1, 0, 0x82686005
wed, z3, 19:59, 19:59, 1, 0, 0x7b66c146
thu, z3, 09:42, 16:58, 0, 0, 0x2e545201
fri, z3, 19:14, 22:35, 0, 0, 0x6095e050
sat, z1, 07:00, 22:39, 0, 0, 0x511ccb0c
mon, z3, 13:05, 16:31, 1, 0, 0x11fca823
thu, z2, 20:31, 23:58, 1, 1, 0x183893db
tue, z1, 01:47, 02:49, 0, 1, 0x3579d687
wed, z1, 00:17, 10:36, 0, 0, 0xdaa2d4ea
sat, z3, 12:55, 14:56, 0, 1, 0x7045a5d4
thu, z1, 04:25, 23:52, 0, 1, 0xc9bf344a
sat, z2, 14:29, 20:47, 1, 1, 0x21feaeb2
wed, z3, 15:11, 21:48, 0, 1, 0x70a45ad3

```
