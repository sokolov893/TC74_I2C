#include <xc.h>

#define _XTAL_FREQ 4000000  

#define TC74_ADDRESS_WRITE 0x9A 
#define TC74_ADDRESS_READ 0x9B 

#define LED1 PORTBbits.RB0
#define LED2 PORTBbits.RB1
#define LED3 PORTBbits.RB2
#define LED4 PORTBbits.RB3
#define LED5 PORTBbits.RB4
#define LED6 PORTBbits.RB5

#define REL RA0
#define SCL RC3    
#define SDA RC4    

void I2C_Init(void) {
    TRISCbits.TRISC3 = 0;  
    TRISCbits.TRISC4 = 1;  
}

void I2C_Start(void) {
    TRISCbits.TRISC3 = 0;  
    TRISCbits.TRISC4 = 0;  
    SDA = 1;
    SCL = 1;
    __delay_us(100);       
    SDA = 0;              
    __delay_us(100);
    SCL = 0;               
}

void I2C_Stop(void) {
    TRISCbits.TRISC4 = 0;  
    SCL = 0;
    SDA = 0;               
    __delay_us(20);
    SCL = 1;               
    __delay_us(20);
    SDA = 1;               
}


unsigned short I2C_Write(unsigned char data) {
    for (char i = 0; i < 8; i++) {
        SDA = (data & 0x80) >> 7;  
        SCL = 1;                   
        __delay_us(20);             
        SCL = 0;                  
        data <<= 1;                
    }
    
    TRISCbits.TRISC4 = 1;        
    SCL = 1;                     
    __delay_us(20);               
    unsigned char ack = SDA;     
    SCL = 0;                    
    TRISCbits.TRISC4 = 0;      

    return ack;                 
}

unsigned short I2C_Read(void) {
    unsigned short data = 0; 
    TRISCbits.TRISC4 = 1;    

    for (char i = 0; i < 8; i++) {
        SCL = 1;                 
        __delay_us(20);         
        data = (data << 1) | SDA; 
        SCL = 0;                  
    }

    TRISCbits.TRISC4 = 0;        
    SDA = 1;                    
    SCL = 1;                    
    __delay_us(20);              
    SCL = 0;                     

    return data;                 
}


unsigned short TC74_Read_Temperature(void) {
    unsigned short temp_read;
    I2C_Start();
    I2C_Write(TC74_ADDRESS_WRITE);  
    I2C_Write(0x00);                
    I2C_Start();                     
    I2C_Write(TC74_ADDRESS_READ);    
    temp_read = I2C_Read();          
    I2C_Stop();
    return temp_read;
}


void Update_LEDs(unsigned short temperature) {
   
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;

    if (temperature >= 30 && temperature <= 34.9) {
        LED1 = 1;
    } else if (temperature >= 35 && temperature <= 39.9) {
        LED1 = 1;
        LED2 = 1;
    } else if (temperature >= 40 && temperature <= 44.9) {
        LED1 = 1;
        LED2 = 1;
        LED3 = 1;
    } else if (temperature >= 45 && temperature <= 49.9) {
        LED1 = 1;
        LED2 = 1;
        LED3 = 1;
        LED4 = 1;
    } else if (temperature >= 50 && temperature <= 54.9) {
        LED1 = 1; 
        LED2 = 1;
        LED3 = 1;
        LED4 = 1;
        LED5 = 1;
    } else if (temperature >= 55 && temperature <= 60) {
        LED1 = 1; 
        LED2 = 1;
        LED3 = 1;
        LED4 = 1;
        LED5 = 1;
        LED6 = 1;
    }
}

void main(void) {
    unsigned short temperature; 
    TRISB = 0x00;
    TRISA = 0x00;
    I2C_Init();    

    while (1) {
        temperature = TC74_Read_Temperature(); 
        Update_LEDs(temperature); 
        if (temperature == 50)   {
            REL = 1;
        } else {
            REL = 0;
        } 
        __delay_ms(2000); 
    }
}





