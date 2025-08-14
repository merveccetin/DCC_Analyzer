/*
SAMPLE 1
control kullanımı (if, while, switch):  yok false
data kullanımı : mode, threshold, return mode 3
guarded (koşul içinde kullanım): yok 0
dDCC = 0/3 = 0

 */

int processData(int mode, int threshold) {

    mode = threshold + 1 ;

    return mode;
}


/*
SAMPLE 2
control kullanımı : 1 has_true
data kullanımı : mode = 0, mode =1 , return mode 2
guarded (koşul içinde kullanım) : 1
dDCC = 1/2 = 0.5

*/

int sample2(int mode)
{
    if(mode > 5)
    {
        mode = 0;
    }

    mode = 1;

    return mode;
}

/*
SAMPLE 3
control kullanımı : 1 has_true
data kullanımı : mode = 0, return mode 2
guarded (koşul içinde kullanım) : 1
dDCC = 1/2 = 0.5

*/

int sample3(int mode)
{
    if(mode > 5)
    {
        mode = 0;
    }


    return mode;
}


/*
SAMPLE 4
control kullanımı : 3 has_true
data kullanımı (guarded + unguarded): mode = 1,  mode = 1, mode = 0, return mode
guarded (koşul içinde kullanım) : 3
dDCC = 3 / 4 = 0.75

*/

int sample4(int mode)
{
    if(mode > 5) // mode: CONTROL (data use sayılmaz)
    {
        mode = 2; //guarded
    }

    else if(mode < 3)
    {
        mode = 1; // guarded
    }

    else
    {
        mode = 0; //guarded
    }

    return mode; //unguarded
}


