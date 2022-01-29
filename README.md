# MiniC
A compiler for a mini C language created with Flex/Bison in C++.

<br>It supports arithmetic calculations as well as logical operations. 
It accepts variable declaration and initialization, if, if-else, while, sqrt() function, pow() function, user-defined functions and function calls.

<br>The input file to be compiled 

    fact =1;
    i=1;
    N=5;

    if ( N > -1 ){
        if ( N == 0 ){
            fact =1;
        }
        else{
            i =1;
            while ( i<= N ){
                fact = fact *i;
                i = i +1;
            }
        }
    }

    n= 5;
    a1 = 1;
    step =1;

    function SumArithSeries(terms,firstterm,step){
        sum=0;
        sum = (terms*(2*firstterm+(terms-1) *step))/2;

        return sum;
    }

    arithseriessum = SumArithSeries(n,a1,step);


    a = 2;
    b = 3;
    c = sqrt(pow(a,2)+pow(b,2));

    pow(3, 3);
