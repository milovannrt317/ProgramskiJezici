/*Milovan Lazic NRT-3/17*/
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<Windows.h>
#define DODELA "\nNije uspela dinamicka dodela memorije!\n"		  // definisanje makroa
#define PROMENA "\nNije uspela dinamicka promena memorije!\n"		
#define OTVARANJE "\nNije uspelo otvaranje datoteke!\n"				
#define RED 80														
#define ODGOVORI 4													
#define MAX 10														
#define TOP 5
using namespace std;


/*Niz sa imenima datoteka koje su u opticaju za izbor.*/
char *oblasti[]={
	"muzika.txt",
	"sport.txt",
	"kultura.txt"
};

/*Ugradjena kratka funkcija greske, koja se koristi za 
iskljucenje programa ako dodje do greske pri otvaranju datokteka ili pri neuspeloj dinamickoj dodeli memorije*/
inline void greska(char *poruka){
	cerr<<poruka;
	exit(1);
}

/*Definicija strukture, koja sadrzi broj pitanja, postavku pitanja, punudjene odgovore i tacan odgovor*/
struct pitanje{
	int brpitanja;
	char postavka[RED+1];
	char odgovori[ODGOVORI][RED+1];
	char tacan;
};


/*Definicija strukture, koja sadrzi ime igraca i poene*/
struct lista{
	char ime[RED+1];
	int poeni;
};

ifstream pit;	/*dodeljen objekt pit klasi ifstream, ovaj objekat sluzi za 
				otvaranje datoteke koja ce iskljucivo citati sadrzaj pitanja sa odgovorima*/

fstream points; /*dodeljen objekta points klasi fstream, ovaj objekat sluzi za 
				otvaranje datoteke koja ce citati i upisivati sadrzaj u tekstualnu datoteku sa poenima igraca*/

/*Deklaracija funkcija*/
void pocetak_kviza();								//ulaz: prihvata ime igraca, poziv iz main(), poziva: izboroblasti, citanjepitanja, kviz, upispoena, top_lista, nastavak_kviza
void izboroblasti();								//ulaz: unos oblasti sa tastature, poziv iz pocetak_kviza
void nastavak_kviza();								//ulaz: unos opcije za nastavak sa tastature, poziv iz pocetak_kviza, poziva u zavisnosti od opcije exit(1) ili pocetak_kviza
void citanjepitanja(struct pitanje *niz);			//ulaz: podaci iz datoteke sa pitanjima, poziv iz pocetak_kviza
int kviz(struct pitanje *niz);						//ulaz: odgovor na pitanja sa tastature, izlaz: broj poena korisnika, poziv iz pocetak_kviza
void upispoena(char *ime, int pts);					//izlaz:na ekran i datoteku "poeni.txt", poziv iz pocetak_kviza
void top_lista(char *ime, int pts);					//ulaz:podaci iz datoteke "poeni.txt", poziv iz pocetak_kviza, poziva: sortira, prikaz_topliste, prikaz_trenutnog_igraca
void sortira(struct lista *niz, int n);				//poziv iz top_lista
void prikaz_topliste(struct lista *niz, int n);		//izlaz: na ekranu, poziv iz top_lista
void prikaz_trenutnog_igraca(char *ime, int pts, struct lista *niz, int n);	//izlaz: na ekranu, poziv iz top_lista
char imeigraca2put[RED+1];
int poeniprosli;
int drugi=0;
int main()
{
	
	/*Poziva se funkcija za pocetak*/
	pocetak_kviza();
	
	

	return 0;
}

/*Funkcija koja je pozvana iz main() funkcije, iz ove funkcije se poziva vecina ostalih funkcija*/
void pocetak_kviza()
{
	
	pitanje *pitanja;

	char imeigraca[RED+1]; 
	int pt;
	//system("Color 0A"); //promena boje
	cout<<"\n\t\t\t\tDobro dosli u kviz pitanja iz odredjenih oblasti"<<endl;
	cout<<"\nPravila igre su sledeca:\n -tacan odgovor donosi 10 poena\n -netacan -5 poena\n -dok \"ne znam\" ne donosi niti oduzima poene"<<endl;

	if(drugi==0){
		/*unos imena, ne prihvata prazan unos*/
		do{
			cout<<"\nUneti ime: ";
			cin.getline(imeigraca,RED+1);
		}while((strlen(imeigraca))==0);
		strcpy(imeigraca2put,imeigraca);
	}
	else
	{
		strcpy(imeigraca,imeigraca2put);
		cout<<"\nDrago nam je da se ponovo igramo sa Vama "<<imeigraca2put<<endl;
	}

	/*poziv funkcije u kojoj korisnik bira oblast pitanja na koje ce odgovarati*/
	izboroblasti();

	
	pitanja= new struct pitanje[MAX];  //niz struktura koja se popunjavaju pitanjima iz tekstualnih datoteka
	if(!pitanja)					  //provera dinamicke dodele
		greska(DODELA);

	/*poziv, funkcije za popunjavanje niza struktura, argumet je niz strukture koji se popunjava*/
	citanjepitanja(pitanja);

	/*poziv funkcije koja vraca broj poena, ova funkcija daje pitanja 
	na ekranu na koja korisnik odgovara, argument je niz struktura sa pitanjima*/
	pt=kviz(pitanja);
	system("cls");			//funckija za ciscenje sadrzaja ekrana(konzole)
	
	/*poziv funkcije za upis poena u tekstualnu datoteku i prikaz na ekranu ostvaren broj poena, argumenti su ime igraca i broj poena*/
	upispoena(imeigraca, pt);
	
	
	delete pitanja;		//brisanje niza struktura sa pitanjima
	
	/*poziv funkcije koja ce prikazati top listu igraca, argumeti su ime igraca i broj poena 
	koji su potrebni za poziv druge funkcije koja prikazuje poziciju na sortiranoj listi sa poenima*/
	top_lista(imeigraca, pt);
	
	/*Poziv funkcije u kojoj se korisnik pita da li zeli da nastavi sa kvizom, 
	ako nastavlja iz funkcije nastavak_kviza se poziva ova funkcija (pocetak_kviza) i krece se ispocetka*/
	nastavak_kviza();
	
}

void izboroblasti()
{
	char izbor[RED+1];
	int oblast;
	
	/*Korisnik bira oblast iz koje ce dobiti pitanja*/
	do{
		
		cout<<"\nMolim Vas izaberite zeljenu oblast sa pitanjama iz (muzika, sport i kultura):"<<endl;
		cin>>izbor;
		
		if(!((strcmp(izbor,"muzika")==0 || strcmp(izbor,"sport")==0 ||strcmp(izbor,"kultura")==0)))
			cerr<<"Niste uneli odgovarajucu opciju za izbor ponudjenih oblasti!"<<endl;
		if((strcmp(izbor,"muzika")==0)) 
			oblast=1;
		else if((strcmp(izbor,"sport")==0))
			oblast=2;
		else if((strcmp(izbor,"kultura")==0))
			oblast=3;
		
	}while(!((strcmp(izbor,"muzika")==0) || (strcmp(izbor,"sport")==0) || (strcmp(izbor,"kultura")==0)));
	
	/*U zavisnosti od izbra otvara se odredjena tekstualna datoteka sa pitanjima*/
	switch(oblast)
	{
	case 1:
		pit.open(oblasti[oblast-1]);
		if(!pit)
			greska(OTVARANJE);
		break;
	case 2:
		pit.open(oblasti[oblast-1]);
		if(!pit)
			greska(OTVARANJE);
		break;
	case 3:
		pit.open(oblasti[oblast-1]);
		if(!pit)
			greska(OTVARANJE);
		break;
	}
}

void citanjepitanja(struct pitanje *niz)
{
	char broj[5];
	/*citanje pitanja i popunjavanje niza strukture pitanja*/
	for(int i=0;i<MAX;i++)
	{
		
		pit.get(broj, 5, '.');
		niz[i].brpitanja=atoi(broj);
		pit.ignore(1, '.');
		pit.getline(niz[i].postavka, RED+1);
		
		for(int j=0;j<ODGOVORI;j++)
			pit.getline(niz[i].odgovori[j], RED+1);
		
		niz[i].tacan=pit.get();
	}
	pit.close(); //po zavrsetku citanja zatvaranje datoteke za citanje
}

int kviz(struct pitanje *niz)
{
	int i, rbr=1, x[10], l=0, f, pts=0;
	char odg[RED];
	srand (time(NULL));     //funkcija za generisanje random brojeva koristi vreme za slucajan izbor
	system("cls");
	 /*Nasumicno postavlja pitanja iz niza pitanja*/
	 while(1){
		f=0;
		i=rand()%10;    
		x[l]=i;
		
		if(l>=1)
		{
			for(int k=0;k<l;k++)
				if(x[k]==i)
				{
					f=1;
					l--;
				}
		} 
		l++;
		
		if(f==0)
		{
			cout<<"\n"<<rbr<<"."<<niz[i].postavka<<endl;
			
			for(int j=0;j<ODGOVORI;j++)
				cout<<niz[i].odgovori[j]<<endl;
			rbr++;
			
			/*Unos odgovora i provera tacnog odgovara gde se povecavaju/smanjuju poeni*/
			do{
				cout<<"\nUneti odgovor(a, b, c ili d): ";
				cin>>odg;
				tolower(odg[0]);
				if(!((strcmp(odg,"a")==0) || (strcmp(odg,"b")==0) || (strcmp(odg,"c")==0) || (strcmp(odg,"d")==0)))
					cerr<<"\nUneli ste nepostojeci odgovor. Molim vas unesite jedan od ponudjenih odgovora (slovo a, b, c ili d)!"<<endl;
			}while(!((strcmp(odg,"a")==0) || (strcmp(odg,"b")==0) || (strcmp(odg,"c")==0) || (strcmp(odg,"d")==0)));
			
			if(odg[0]==niz[i].tacan)
			{
				pts+=10;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //promena boje
				cout<<"Tacan odgovor!"<<endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			else if(odg[0]=='d')
			{
				pts+=0;
				cout<<"Tacan odgovor je: "<<niz[i].tacan<<endl;
			}
			else
			{
				pts-=5;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				cout<<"Netacan odgovor!"<<endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				cout<<"Tacan odgovor je: "<<niz[i].tacan<<endl;
			}
		}
		else
			continue;

		
		if(rbr==11)
				break;
	}
	return pts;
}

void upispoena(char *ime, int pts)
{
	/*otvaranje datoteke za upis podataka u nastavku sadrzaja*/
	points.open("poeni.txt", ios::app);
		if(!points)  //provera otvaranja datoteke
			greska(OTVARANJE);
	cout<<"\n"<<ime<<" Vas broj poena je: "<<pts<<endl;  //prikaz na ekranu broj poena
	
	if(drugi==0)
		poeniprosli=pts;
	else
	{
		if(poeniprosli<pts)
			cout<<"\nBravo odradili ste kviz bolje nego u prethodnom pokusaju!"<<endl;
		else if(poeniprosli>pts)
			cout<<"\nSteta pokvarili ste rezultat u odnosu na prethodni pokusaj!"<<endl;
		else
			cout<<"\nOdradili ste kviz sa istim brojem poena kao u prethodnom pokusaju!"<<endl;
		poeniprosli=pts;
	}
	/*U zavisnosti od broja poena izbacuje odgovarajucu poruku*/
	if(pts>=90)
		cout<<"\nSvaka cast, inzvaredan broj poena!"<<endl;
	else if(pts>=70 && pts<90)
		cout<<"\nDobar broj poena."<<endl;
	else if(pts>=40 && pts<70)
		cout<<"\nMoze to mnogo bolje, los broj poena."<<endl;
	else
		cout<<"\nLos broj poena!"<<endl;

	points<<"\n"<<ime<<"-"<<pts;   //upis imena i poena u tekstualnu datoteku
	points.close();  //zatvaranje datoteke sa poenima
}

void nastavak_kviza()
{
	
	int  nastavak;
	char izbor[RED+1];

	do{
		cout<<"\nZelite li da nastavite sa kvizom (da/ne):"<<endl;
		cin>>izbor;
		
		if(!(strcmp(izbor,"da")==0 || strcmp(izbor,"ne")==0))
			cerr<<"Niste uneli odgovarajucu opciju za izbor, molimo unesite \"da\" ukoliko nastavljate sa kvizom ili\n\"ne\" ako odlazite iz kviza!"<<endl;
		if((strcmp(izbor,"da")==0)) 
			nastavak=1;
		else if((strcmp(izbor,"ne")==0)) 
			nastavak=2;
		
	}while(!((strcmp(izbor,"da")==0) || (strcmp(izbor,"ne")==0)));
	cin.get();
	switch(nastavak)			//u zavisnosti od unete opcije startuje ponovo kviz/prekida program
	{
		case 1: system("cls"); 
			drugi=1;
			pocetak_kviza(); 
			break;
		case 2:  system("cls"); 
			cout<<"Hvala sto ste se igrali sa nama.\nDodji te nam ponovo."<<endl; 
			exit(1); 
			break;
	}
}
void top_lista(char *ime, int pts)
{
	lista *spisak;
	int br=0, pom;
	char tmp[RED];
	spisak=new struct lista[100];		//lista igraca
		if(!spisak)						//provera dinamicke dodele
			greska(DODELA);
	
	points.open("poeni.txt", ios::in);	//otvaranje datoteke za citanje poena
	if(!points)							//provera otvaranja datoteke
		greska(OTVARANJE);
	while(!points.eof())				//citanje podataka sve do kraja same datoteke i upis u niz struktura
	{
		
		points.getline(spisak[br].ime, RED+1, '-');
		points>>spisak[br++].poeni;
		points.get();
	}

	/*Poziv funkcije za sortiranje spiska*/
	sortira(spisak, br);
	

	/*Poziv funkcije za prikaz pozicije na listi, igraca koji trenutno igra*/
	prikaz_trenutnog_igraca(ime,pts, spisak, br);
	

	/*Poziv funkcije za prikaz top liste*/
	prikaz_topliste(spisak, br);

	points.close();   //zatvaranje datoteke za citanje poena
	delete spisak;    //brisanje niza struktura u kome su upisana imena i broj poena
}

void sortira(struct lista *niz, int n)
{
	lista tmp;
	
	/*Sortiranje spiska u opadajacem poretku prema poenima*/
	for(int i=0;i<n-1;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			if(niz[i].poeni<niz[j].poeni)
			{
				tmp=niz[i];
				niz[i]=niz[j];
				niz[j]=tmp;
			}
		}
	}
}

void prikaz_topliste(struct lista *niz, int n)
{
	/*Ako je broj u spisku igraca koji su iscitani iz datoteke sa poenima veci od 5 onda ce se 
	dostaviti poruka na ekranu da postoji Top 5 lista, u suprotnom obavestava korisnika da nema Top 5 liste ali prikazuje najbolje igrace*/
	if(n>TOP)
	{	
		n=TOP;
		cout<<"\nTop "<<TOP<<" lista je:"<<endl;
	}
	else
		cout<<"\nTrenutno u bazi nema igraca za prikaz top "<<TOP<<" liste, ali prikazacemo najbolje:"<<endl;
	
	
	for(int i=0;i<n;i++)
		cout<<i+1<<". "<<niz[i].ime<<"-"<<niz[i].poeni<<endl;
}
void prikaz_trenutnog_igraca(char *ime, int pts, struct lista *niz, int n)
{
	int *trenutna_poz, br=0;
	
	trenutna_poz= new int[100];			//niz za upis pozicija igraca 
	if(!trenutna_poz)					//provera dinamicke dodele memorije
		greska(DODELA);

	/*upisivanje pozicije u niz*/
	for(int i=0;i<n;i++)
		if((strcmp(niz[i].ime,ime)==0) && niz[i].poeni==pts)
			trenutna_poz[br++]=i;
	/*ako ima vise istih imena sa istim brojem poena, prikazuje sva imena i njihove pozicije na rang  listi,
	ako je jedinstveno ime prikazace poziciju tog imena na rang listi*/
	if(br>1)
	{
		cout<<"\nPostoji vise imena kao trenutno uneto, sa istim brojem poena!"<<endl;
		for(int i=0;i<br;i++)
		cout<<ime<<" - "<<pts<<" Pozicija je "<<trenutna_poz[i]+1<<"."<<endl;
	}
	else
	{
		cout<<"\nVase ime, broj poena i mesto koje ste zauzeli na listi je:"<<endl;
		cout<<ime<<" - "<<pts<<" Pozicija je "<<trenutna_poz[0]+1<<"."<<endl;

		/*U zavisnosti od pozicije na kojoj je trenutni korisnik izbacuje odgovarajucu poruku*/
		if (trenutna_poz[0]+1 >= 1 && trenutna_poz[0]+1 <= 10 && n>=10)
			cout<<"\nOdlicno nalazite se u top 10 najboljih igraca."<<endl;
		else if (trenutna_poz[0]+1 >= 11 && trenutna_poz[0]+1 <= 20 && n>=20) 
			cout<<"\nVrlo dobro nalazite se u top 20 igraca."<<endl;
		else if (trenutna_poz[0]+1 >= 21 && trenutna_poz[0]+1 <= 50)
			cout<<"\nNalazite se izmedju 21 i 50 pozicije. Moze to bolje!"<<endl;
		else if(trenutna_poz[0]+1 >= 50)
			cout<<"\nLose ste se pokazali na listi!"<<endl;
	}


	delete trenutna_poz;		//brise niz sa pozicijama
}