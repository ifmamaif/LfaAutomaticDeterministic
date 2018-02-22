// Begin include libraries
#include<stdio.h>
#include<stdlib.h>
// END include libraries
// BEGIN define structs
struct stariurmatoare{
	int stare;
	char c[1];
	struct stariurmatoare *next;
};
struct automat{ // "am discutat deja problema celor 2 structuri" => o sa am grija pe viitor sa nu am structuri inplus
	struct stariurmatoare *legatura;
};
//END define structs

void afisare(struct automat *cod,int n){
	int i=0;
	for(;i<n;i++){
		printf("%d :\n",i);
		struct stariurmatoare* legatura;
		legatura=cod[i].legatura;
		while(legatura!=NULL){
			printf("	%c %d\n",legatura->c[0],legatura->stare);
			legatura=legatura->next;			
		}
	}
}

int cautastarefinala(struct automat* cod,int stare,int *vizitati,int starifinale){
	if(stare>=starifinale) {	// este stare finala 
		return 2; 
	}
	if(vizitati[stare]==1){	 // a gasit o stare deja vizitat => intoarcere	
		return 0;
	}
	if(cod[stare].legatura==NULL){ // nu exista succesori
		return 1;
	}	
	vizitati[stare]=1; 
	struct stariurmatoare *legatura;
	legatura=cod[stare].legatura;
	while(legatura!=NULL){
		if(cautastarefinala(cod,legatura->stare,vizitati,starifinale)==2) // daca s-a ajuns la stare finala
			return 2; 
		vizitati[stare]=0;
		legatura=legatura->next;
	}
	if(stare>=starifinale) return 2; 
	return 1; // nu s-a gasit stare finala
}

int cautaciclu(struct automat* cod,int stare,int *vizitati,int n,int starifinale){
	if(vizitati[stare]==1)return 0; //daca a gasit o stare finala se intoarce la rezult
	if(cod[stare].legatura==NULL)return 1; //daca nu are succesori se se intoarce la rezult
	vizitati[stare]=1; // monitorizez starile pe care le vizitez
	struct stariurmatoare *legatura; // auxiliar pentru parcurgerea succesorilor
	legatura=cod[stare].legatura;
	while(legatura!=NULL){ // parcurgerea succesorilor din starea curenta
		int rezult=cautaciclu(cod,legatura->stare,vizitati,n,starifinale); // rezultatul de la primele verificari
		if(rezult==0){		// daca e 0 a gasit ciclu cautam mai departe daca gasim o stare finala de la starea curenta
			int *vizitati2;
			vizitati2=(int*)malloc(n*sizeof(int));
			{
				int i=0;
				for(;i<n;i++)
					vizitati2[i]=0;
			}
			if((rezult=cautastarefinala(cod,stare,vizitati2,n-starifinale))==2){
				 free(vizitati2);	
				 return -1; // a gasit stare finala => limbaj infinit
			}		
			
		}
		if(rezult==-1){						
				return -1;
		}
		vizitati[stare]=0; // corectam starile vizitate pentru posibile vizite viitoare
		legatura=legatura->next; // mergem mai departe in lista de succesori
	}
	return 1; // limbajul este finit 
}

int elibstare(struct stariurmatoare *road){ //help to free
	if(road!=NULL){
		elibstare(road->next);
		free(road);
		return 0;
	} 
	return 0;
}

int elibarare(struct automat *cod,int n,int i){ // help to free
	if(i<n){
		elibarare(cod,n,i+1);
		elibstare(cod[i].legatura);
		return 0;
	}
	return 0;
}

// BEGIN main
int main(int argc, char *argv[]){
	FILE * file = fopen(argv[1], "r");
    if(file==NULL)printf("This file doesn't exist!"),exit(0);	
	char *w;
	//READ first line
	{
		char c[1];
		int length=0,endfile=1;
		while((endfile=fread(c,1,1,file))>0 && (c[0]!=' '&& c[0]!='\n' && c[0]!='\r')){
			length++;			
		}
		w=(char*)malloc(length*sizeof(char));
		rewind(file);
		fread(w,length,1,file);
		w[length]='\0';
		while((endfile=fread(c,1,1,file))>0 && (c[0]!='\n' && c[0]!='\r')){		
		}
	}
	//END READ first line
	struct automat *cod;
	cod=NULL;
	// READ second line
	int n; // numar stari
	fscanf(file,"%d",&n); 
		cod=(struct automat*)malloc(n*sizeof(struct automat));
		int i;
		for(i=0;i<n;i++)
			cod[i].legatura=NULL;
	int nf; // numar stari finale
	fscanf(file,"%d",&nf);
	
	int s; // numar simboluri
	fscanf(file,"%d",&s);
	//END READ second line
	
	//READ rest of file
	{		
		int st=0,end=0; //st-stare curenta , end=urmatoarea stare
		char c[3]; // simbolul de tranzitie
		while (fscanf(file,"%d",&st)>0 && fread(c,sizeof(char),3,file)>0 && fscanf(file,"%d",&end)>0){					
			if(cod[st].legatura==NULL){
				cod[st].legatura=(struct stariurmatoare*)malloc(sizeof(struct stariurmatoare));
				cod[st].legatura->stare=end;
				cod[st].legatura->c[0]=c[1];
				cod[st].legatura->next=NULL;
			}else{				
				struct stariurmatoare *legatura;
				legatura=cod[st].legatura;
				while(legatura->next!=NULL){
					legatura=legatura->next;
				}
				{
					struct stariurmatoare *auxiliar;
					auxiliar=(struct stariurmatoare*)malloc(sizeof(struct stariurmatoare));
					auxiliar->stare=end;
					auxiliar->c[0]=c[1];
					auxiliar->next=NULL;
					legatura->next=auxiliar;
				}
			}		
		}
		fclose(file);		
	}
	//END READ rest of file
	if(w[0]=='_'){
		int ok=0;
		int *vizitati;
		vizitati=(int*)malloc(n*sizeof(int));
		{
			int i=0;
			for(;i<n;i++)
				vizitati[i]=0;
		}
		ok=cautaciclu(cod,0,vizitati,n,nf);	
		free(vizitati);
		ok=ok<=0?0:1;
		printf("%d",ok); // 1=limbaj finit | 0=altfel;
	} 
	else {
		int i=0;
		int ok=1;
		int stare=0;
		while(w[i]!='\0' && ok==1){
			struct stariurmatoare * legatura;
			legatura=cod[stare].legatura;
			while(legatura!=NULL && legatura->c[0]!=w[i]){
				legatura=legatura->next;
			}
			if(legatura==NULL)
				ok=0; 
			else			
			{					
				stare=legatura->stare;
				i++;
			}
		}
		if(w[i]=='\0' && stare>=(n-nf))
			printf("1");
		else
			printf("0");
	}
	elibarare(cod,n,0); //adaugat acuma
	free(cod);
	free(w);	
	return 0;
}
//END main