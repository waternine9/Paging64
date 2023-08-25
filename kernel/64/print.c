#include "print.h"
#include "../include.h"

/* "stolen" from my old kernel */
typedef struct {
    char *Dest;
    uint64_t N;
    uint64_t I;
} destination;

typedef struct {
    uint64_t Number;
    uint64_t Sign;
} num;

const char *BASE_CHARS = "0123456789ABCDEF";

static num Int2Num(int i) {
    return (num){(i < 0) ? -i : i, i < 0};
}
static num Uint2Num(unsigned int i) {
    return (num){i, 0};
}
static void WriteDestination(destination *Dest, char C)
{
    if (Dest->I < Dest->N) {
        Dest->Dest[Dest->I] = C;
    }
    Dest->I++;
}
static void WriteDestinationStr(destination *Dest, const char *S)
{
    while (*S) {
        WriteDestination(Dest, *S++);
    }
}
static void WriteDestinationStrN(destination *Dest, const char *S, int N)
{
    for (int I = 0; S[I] && I < N; I++) {
        WriteDestination(Dest, S[I]);
    }
}
static int NumWidthBaseN(size_t Value, size_t Base)
{
    if (Base == 1) return 0;
    if (Base > 16) return 0;

    size_t End = 0;
    do {
        Value /= Base;
        End += 1;
    } while (Value);

    return End;
}
static void WriteDestinationBaseN(destination *Dest, size_t Value, size_t Base)
{
    if (Base == 1) return;
    if (Base > 16) return;

    size_t End = NumWidthBaseN(Value, Base);

    char Buf[64] = { 0 };
    do {
        size_t Index = Value % Base;
        Value /= Base;

        Buf[--End] = BASE_CHARS[Index];
    } while (Value);

    WriteDestinationStr(Dest, Buf);
}
static int IsDigit(int c)
{
    return c >= '0' && c <= '9';
}
static int DigitValue(int c)
{
    return c - '0';
}
static void Number(destination *Dest, int Width, int ZeroPad, num Num, int Base)
{
    if (Num.Sign) {
        WriteDestination(Dest, '-');
    }

    int L = NumWidthBaseN(Num.Number, Base);
    for (; L < Width; L++) {
        if (ZeroPad) WriteDestination(Dest, '0');
        else         WriteDestination(Dest, ' ');
    }

    WriteDestinationBaseN(Dest, Num.Number, Base);
}
void kprint(char *Dest, size_t N, const char *Fmt, ...)
{
    if (N == 0) return;

    va_list Va;
    va_start(Va, Fmt);

    destination Buffer = { 0 };
    Buffer.Dest = Dest;
    Buffer.N = N-1;
    int Width = 0;
    int ZeroPad = 0;
    int Precision = 0;

    for (int I = 0; Fmt[I]; I++) {
        Width = 0;
        switch (Fmt[I]) {
            case '%':
                I++;
                if (Fmt[I] == '0') {
                    ZeroPad = 1;
                    I++;
                }
                while (IsDigit(Fmt[I])) {
                    Width += DigitValue(Fmt[I++]);
                }
                if (Fmt[I] == '.' && Fmt[I+1] == '*') {
                    Precision = va_arg(Va, int);
                    I += 2;
                }
                switch (Fmt[I]) {
                    case 'c':
                        WriteDestination(&Buffer, va_arg(Va, int));
                        break;
                    case 'd':
                        Number(&Buffer, Width, ZeroPad, Int2Num(va_arg(Va, int)), 10);
                        break;
                    case 'u':
                        Number(&Buffer, Width, ZeroPad, Uint2Num(va_arg(Va, unsigned int)), 10);
                        break;
                    case 'x':
                        Number(&Buffer, Width, ZeroPad, Uint2Num(va_arg(Va, unsigned int)), 16);
                        break;
                    default:
                    case '%':
                        WriteDestination(&Buffer, '%');
                        break;
                }
                break;
            default:
                WriteDestination(&Buffer, Fmt[I]);
                break;
        }
    }

    WriteDestination(&Buffer, '\0');

    va_end(Va);
}
/* end of stealing */