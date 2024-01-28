#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

/*
/\/\/\/\/\/\/\/\/\IMPLEMENTATION OF *SIGNED* BIGINT 1024 BIT INTEGERS WITH ADDITION, SUBTRACTION AND MULTIPLICATION/\/\/\/\/\/\/\/\/\

*LIMIT* : -10^309 to 10^309 
Using int datatype to store the BigInt bits. To get maximum memory efficiency we can take the base of INT_MAX.
Although the problem with this would be conversion, as it would require division of BigInt with INT_MAX for bit conversion. 

So used the BASE as 10^9 as there would not be a need of bit conversion. Just grouping the number to 9 digits and 
storing as elements in array of integers would suffice. And also because 10^9 is the closest to INT_MAX
*/


#define DIGITS 309 //Maximum number of digits that can be represented by 1024 bits BigInt~||||(1024log(2))
#define SIZE 36    //Size of the array to be used to represent BigInt with Base= 10^9 ~|||||||(309/9)
#define BASE 1000000000 //Base of the representation(max size of each element of the array)~||(10^9)

typedef struct BigInt_tag{
    int number[SIZE];
    int sign;   //-1 corresponds to negative and +1 corresponds to positive
    int size;   //gives the size of the filled array.

}BigInt;

BigInt rm1scompliment(BigInt);
//Function for finding the (r-1)'s compliment with base 10^9. Used in Subtraction and Addition function
BigInt Add(BigInt, BigInt);
//For Adding two SIGNED BigInt numbers, prints error if overflow encountered   
BigInt TakeInput();  
//Takes input in the form of strings and stores pack of 9 digits in each element of array along with sign and size, 
//returns the BigInt Structure, if 309 digit limit exceeded, prints error statement
BigInt Subtract(BigInt, BigInt);
//For Subtracting two SIGNED BigInt numbers.
BigInt Multiply(BigInt, BigInt);
//For Multiplying two SIGNED BigInt numbers.
void Display(BigInt);
//Displayes the BigInt along with the sign and the size of the array

BigInt Multiply(BigInt a, BigInt b){
    printf("Multiplying.........\n");
    BigInt answer;
    for(int i=0;i<SIZE;i++){
        answer.number[i]=0;
    }
    int j=0;
    for(int i=0;i<b.size;i++){
        long long carry=0;
        int lim = i+a.size;
        for(j=i;j<lim;j++){
            long long temp;
            temp = (long long)answer.number[j]+ (long long)b.number[i]*a.number[j-i] + carry;
            answer.number[j] = (int)(temp%BASE);
            carry = (int)(temp/BASE);
        }
        if(carry!=0){
            answer.number[j] = (int)carry;
            j++;
        } 
    }
    answer.sign = a.sign*b.sign;
    answer.size = j;
    return answer;

    
}

BigInt rm1scompliment(BigInt b){
    for(int i=0;i<b.size;i++){
        b.number[i] = BASE - b.number[i]-1;
    }
    
    return b;
}

BigInt Subtract(BigInt a, BigInt b){
    BigInt answer;

    if(a.sign*b.sign==1){
        //same sign subtraction using r-1's compliment
        int i;
        for(i=0;i<b.size;i++){
            b.number[i] = BASE - b.number[i]-1;
        }
        while(i<a.size){
            b.number[i] = BASE-1;
            i++;
        }
        b.size = (a.size > b.size) ? a.size : b.size;
        long long carry=0;
        i=0;
        while(i<a.size ){
            answer.number[i]=(a.number[i]+b.number[i]+carry)%BASE;
            carry = (a.number[i]+b.number[i]+carry) - answer.number[i];
            carry = carry/BASE;
            i++;
        }
        while(i<b.size){
            answer.number[i]=(b.number[i]+carry)%BASE;
            carry = (b.number[i]+carry) - answer.number[i];
            carry = carry/BASE;
            i++;
        }
        
        answer.size = i;
        if(carry==0){
            answer=rm1scompliment(answer);
            answer.sign=(-1)*a.sign;
        }
        else{
            answer.sign = a.sign;
            BigInt one;
            one.size=1;
            one.number[0]=1;
            one.sign=1;
            // calling Add function to add 1 to answer
            answer = Add(answer,one);
            
        }
    }
    else{
        //Opposite sign subtraction is equivalent to Addition
        //Calling Add function with proper sign manipulation
        b.sign=(-1)*b.sign;
        answer = Add(a,b);
    }
    return answer;
}

BigInt Add(BigInt a, BigInt b){
    BigInt answer;
    int i=0;
    long long carry=0;
    if(a.sign*b.sign==1){
        //Same sign addition
        while(i<a.size && i<b.size){
            answer.number[i]=(a.number[i]+b.number[i]+carry)%BASE;
            carry = (a.number[i]+b.number[i]+carry) - answer.number[i];
            carry = carry/BASE;
            i++;
        }
        while(i<a.size){
            answer.number[i]=(a.number[i]+carry)%BASE;
            carry = (a.number[i]+carry) - answer.number[i];
            carry = carry/BASE;
            i++;
        }
        while(i<b.size){
            answer.number[i]=(b.number[i]+carry)%BASE;
            carry = (b.number[i]+carry) - answer.number[i];
            carry=carry/BASE;
            i++;
        }
        if(carry!=0){
            if(i==SIZE) printf("OVERFLOW ENCOUNTERED\n");
            else{
                answer.number[i]=carry;
                i++;
            }     
        }
        answer.sign=a.sign;
        answer.size=i;
    }
    else{
        //different sign addition is equivalent to Subtraction
        //Calling subtraction function with proper sign manipulation
        b.sign=b.sign*(-1); // to change the sign
        answer = Subtract(a,b);

    }
    
    return answer;
}

BigInt TakeInput()
{
    BigInt a;
    printf("Enter the number: ");
    char input[10000];
    scanf("%s",input);
    int i=1;
    if(input[0]=='-') a.sign=-1; 
    else{
        i=0;
        a.sign=1;
    }
    int digits = strlen(input)-i;
    i = strlen(input)-1;
    int count =0;
    int bigindex=0;
    int mult=1;
    a.number[bigindex]=0;
    a.size=1;
    //Divides the string to 9 digits and stores in BigInt structure
    while(digits-- && bigindex<SIZE){
        if(count == 9){
            count=0;
            mult=1;
            bigindex++;
            a.number[bigindex]=0;
            a.size++;
        }
        a.number[bigindex] = a.number[bigindex] + mult*(input[i]-'0');
        mult = mult*10;
        i--;
        count++;
        
    }
    //Check for overflow condition
    if(bigindex==SIZE) printf("Overflow error Encountered");

    return a;
    
}

void Display(BigInt a){
    
    int i=0;
    int size = a.size;
    printf("Your Integer:\n");
    printf("size: %d\n",a.size); // Displays the size of array
    if(a.sign==-1) printf("-");
    size--;
    printf("%d",a.number[size]);
    while(size--){
        //As element "0000x" is stored as "x" in array, adding equivalent zeroes to display
        if(a.number[size]==0) printf("000000000");
        else{
            int digits = (int)log10((double)llabs(a.number[size])) + 1;
            for(int i=0;i<9-digits;i++){
                printf("0");
            }
            printf("%d",a.number[size]);
        }
        
    
    }
    printf("\n");
}


void main(){
    
    int cursor;
    printf("ENTER\n 0-END\n1-ADD\n2-SUBTRACT\n3-MULTIPLY:");
    scanf("%d",&cursor);
    while(cursor!=0){
        BigInt a,b,ans;
        a= TakeInput();
        b = TakeInput();
        if(cursor==1) ans = Add(a,b);
        else if(cursor==2) ans = Subtract(a,b);
        else if(cursor==3) ans = Multiply(a,b);

        Display(ans);
        printf("ENTER\n 0-END\n1-ADD\n2-SUBTRACT\n3-MULTIPLY:");
        scanf("%d",&cursor);

    }
    
}