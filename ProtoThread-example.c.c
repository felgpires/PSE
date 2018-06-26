/*
JOSE HENRIQUE GROTTO
FELIPE GOMES PIRES
DIEGO MARQUES ALVES
*/

#include "pt.h"

#DEFINE max 9
#DEFINE STX 0x02
#DEFINE ETX 0x03

uint8_t data_t;

uint8_t data[9] = {STX, 5, 11, 22, 33, 44, 55, 39, ETX};

int flag;

void protothread_decodifica(struct pt *pt2, uint8_t *byte ){
	
	PT_BEGIN(pt2);
	
	static int qtd;
	static int i;
	static uint8_t buffer[max] = {};
	static uint8_t chk = 0;
	
	while(1){
		
		flag = 1;
		
		PT_WAIT_UNTIL(pt2, flag != 0);
		
		if(*byte == STX){
			printf("STX ack");
			flag = 0;
			
			PT_WAIT_UNTIL(pt2, flag != 0);
			qtd = *byte;
			flag = 0;
			
			PT_WAIT_UNTIL(pt2, flag != 0);
			for(i = 0; i < qtd; i++){
				printf("Byte sendo recebido");
				buffer[i] = *byte;
				chk = chk ^ *byte;
				flag = 0;
				PT_WAIT_UNTIL(pt2, flag != 0);
			}
			
			if(*byte == chk){
				printf("Check Accepted");
				flag = 0;
			}
			else{
				printf("Check Error");
				exit(-1);
			}
			
			PT_WAIT_UNTIL(pt2, flag != 0);
			
			if(*byte == ETX){
				printf("SUCESSFULL COMM");
			}
		}
		flag = 0;
		PT_END(pt2);
	}
}


int main(void){
	
	PT_INIT(&pt2);
	
	int i;
	
	for (i = 0; i < 9; i++){
		protothread_decodifica(data[i]);
		flag = 1;
	} 

}