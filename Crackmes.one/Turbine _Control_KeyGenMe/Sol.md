# The CTF
https://crackmes.one/crackme/69bd737bf2d49d8512f64adc

# The validation logic:
Given a serial key input XXXX-YYYY-ZZZZ-WWWW
the program seperate it to the following parts
XXXX, YYYY, ZZZZ, WWWW
And generates HWID , we will call this ID , Guid here.


## Key validation
The key validation is seperated into 4 tests
while if debugged there are some chnages to the values used
in this readme we will assume not debugged and use these values

## First test
The test is conducted using the Guid, XXXX and a vlaue 31
which is picked because not under debugger.

Psudo:
```bash
for(i = 0; i<4; i++)
    val = 31 XOR Guid[4] XOR ((Guid[i] + 3)& 0xFF)

    r = val % 93

    comp_i = r + 34
    if ((r + 33) < 0x2DU)
        comp_i = r + 33

    if(XXXX[i] != comp_i)
        return False

    i++

return True
```
So eventually using 31 and Guid for i from 0 to 3
we can generate the 4 bytes of XXXX

## Second Test
Given XXXX, YYYY, and a poison value which is 0 if not debugged.
So in our case this poison value will be 0

The test takes XXXX value and spreads it in a way that
each byte of XXXX is now a 0 padded 32bit value
Then performes some shiftings and pair summing in a way that if
XXXX is: 0xAABBCCDD (4 bytes)
then the final XXXX sum is 128bit:
```bash
[(32bit)XXXX[24:31], (32bit)XXXX[16:23], (32bit)(XXXX[8:15] + XXXX[24:31]), (32bit)(XXXX[0:7] + XXXX[16:23])]
```

Then out of this sum XXXX extended value the test creates a seed
Which is just a sum of all the bytes
```bash
Seed = (32bit)(XXXX[8:15] + XXXX[24:31] + XXXX[0:7] + XXXX[16:23])
```

Then the test uses the Seed to calculate the final_XXXX
with a const table of chars which is hardcoded in the binary

```bash
static const uint8_t table[256] =
    "Wj5Lr)A^3hO&[=tIb7U+nCX1|Fl:Ri.Dq9]Ke(s?Vk2N`%zB\\6mH*Tp>Yf0Mx#Z@o"
    "4S~Ga,v;QgE_'u8Jc/}Pj<w)UdAy.Lh5^rC{1Ol&Xt=RiF[3|Hb+n7WDq9e"
    "Ks(]k2V?`N%zB\\6mT*p>fY0xM#Z@o4~SGav,Q;gEu'_8cJ/}Pw<d)yUAlO1"
    "{C^r5h&Xt=RnF|3[bH+iW7Dqe9K]s(k?V`Nz%2Bm6\\Tp*>fxY0M@Z#o~4SaGv,Q;g"
    "E_u'8c}P";

final_XXXX[0] = table[seed mod 256]
for i from 1 to 3
    final_XXXX[i] = table[seed + final_XXXX[i-1]]
```

```bash
final_XXXX[0] = table[seed mod 256]
final_XXXX[1] = table[seed + final_XXXX[0] mod 256]
final_XXXX[2] = table[seed + final_XXXX[1] mod 256]
final_XXXX[3] = table[seed + final_XXXX[2] mod 256]
```

and the condition is essentially
```bash
from i=0 to i=3
    YYYY[i] == final_XXXX[i]
```

So given we already generated XXXX we can now generate YYYY
```bash
seed =  XXXX[0] + XXXX[1] + XXXX[2] + XXXX[3]
YYYY[0] == table[seed mod 256]
for i from 1 to 3
    YYYY[i] = table[seed + YYYY[i-1]]
```

## Third test
The third test operates on ZZZZ and a value that will allways be 5040 (Becasue not debugged)
and tests:
```bash
ZZZZ[0] * ZZZZ[1] == 5040
AND
ZZZZ[2] + ZZZZ[3]  == 150
```

So generating ZZZZ is easy

## Fourth test    
With XXXX, YYYY, ZZZZ, WWWW as input
the test uses XXXX, YYYY, ZZZZ
to calculate value to compare with the WWWW

value start at 0 and is going through the following
```bash
for i from 0 to 3
    value = XXXX[i] + 31 * value

for i from 0 to 3
    value = YYYY[i] + 31 * value

for i from 0 to 3
    value = ZZZZ[i] + 31 * value
```

Then the test read the value
to a buffer uisng
```bash 
sprintf(Buffer, "%04u", value)
```

and we compare the buffer
```bash
for i from 0 to 3   
    WWWW[i] == value[i]
```

So the constrain is that WWWW
should be chosen by using XXXX,YYYY,ZZZZ to calculate it
exaxtly like the test does to to generate the value.
