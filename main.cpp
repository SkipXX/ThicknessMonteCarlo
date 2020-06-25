#include <iostream>
#include <vector>
#include <time.h>
#include <limits>


///---------------------------
#include <random>

typedef std::default_random_engine rng_type;
std::uniform_real_distribution<float> udist(0.0, 1.0);

rng_type rng;
///---------------------------

#define MY_VEC vector<float>

using namespace std;



MY_VEC calc_F(const MY_VEC& P_1,const MY_VEC& P_2,const MY_VEC& P_3){
    MY_VEC P_12={P_2[0]-P_1[0],P_2[1]-P_1[1],P_2[2]-P_1[2]};
    MY_VEC P_13={P_3[0]-P_1[0],P_3[1]-P_1[1],P_3[2]-P_1[2]};


    MY_VEC P_X={P_12[1]*P_13[2]-P_12[2]*P_13[1],P_12[2]*P_13[0]-P_12[0]*P_13[2],P_12[0]*P_13[1]-P_12[1]*P_13[0]};
//    cout << P_13[0] << " " <<  P_13[1] << " " << P_13[2];
//    exit(1);

    float D = -P_X[0]*P_1[0]-P_X[1]*P_1[1]-P_X[2]*P_1[2];

    MY_VEC F = {P_X[0],P_X[1],P_X[2],D};

return F;
}

bool is_Under_F(const MY_VEC& F, const MY_VEC& P){

return (P[2]*F[2]+F[3]) < (-P[0]*F[0]-P[1]*F[1]);
}

int main()
{
    float LO,RO,RU,LU,ZMID;
    float xscale,yscale;
    float XMID,YMID;
    int N=0;
    char temp;
    bool simple=false;
    cout << "Simple(y or n)?";
    cin >> temp;
    if(temp == 'y') simple=true;

    if(simple){
        xscale = 1;
        yscale = 1;
        XMID = 0.5;
        YMID = 0.5;
        N=0;
    }

    cout << "Alle Längenangeben in gleichen Einheiten!\n";

    if(!simple){
    cout << "Länge der Probe(Oben -> Unten;S->N): ";
    cin >> xscale;
    cout << "Breite der Probe(Links -> Rechts;W->O): ";
    cin >> yscale;
    }

    cout << "Dicke an Ecken im Uhrzeigersinn angeben startend mit\nLinks Oben: ";
    cin >>  LO;
    cout << "Rechts Oben: ";
    cin >> RO;
    cout << "Rechts Unten: ";
    cin >> RU;
    cout << "Links Unten: ";
    cin >> LU;
    cout << "Mitte: ";
    cin >> ZMID;

    if(!simple){
    cout << "Position der Mitte?\n";
    cout << "Längenkoordinate(S->N): ";
    cin >> XMID;
    cout << "Breitenkoordinate(W->O): ";
    cin >> YMID;

    cout << "Angabe von Simulationsanzahl(0=default): ";
    cin >> N;
    }

    if(N==0)N=1337*31415;

    rng.seed(time(NULL));
    ///P = Punkt; F = Fläche

    ///L = Links; R = Rechts; O = Oben; U = Unten;
//    constexpr float zscale = 1;
    MY_VEC P_LO={xscale,0,LO};
    MY_VEC P_LU={0,0,LU};
    MY_VEC P_RO={xscale,yscale,RO};
    MY_VEC P_RU={0,yscale,RU};

    MY_VEC P_MID={XMID,YMID,ZMID};




    ///F ( X , Y , Z , D )
    MY_VEC F_L = calc_F(P_LU,P_LO,P_MID);
    MY_VEC F_R = calc_F(P_RO,P_RU,P_MID);
    MY_VEC F_O = calc_F(P_LO,P_RO,P_MID);
    MY_VEC F_U = calc_F(P_RU,P_LU,P_MID);

//    MY_VEC F_TEST = calc_F({0,0,scale},{scale,0,scale},{scale/2,scale/2,scale});
//    MY_VEC F_TEST = calc_F({0,0,0},{scale,0,0},P_MID);

//    cout << F_L[0] << "\t" << F_L[1] << "\t" << F_L[2] << "\t" << F_L[3] << "\n";
//    cout << F_R[0] << "\t" << F_R[1] << "\t" << F_R[2] << "\t" << F_R[3] << "\n";
//    cout << F_O[0] << "\t" << F_O[1] << "\t" << F_O[2] << "\t" << F_O[3] << "\n";
//    cout << F_U[0] << "\t" << F_U[1] << "\t" << F_U[2] << "\t" << F_U[3] << "\n";

    int counter=0;

    for(int ii = 0;ii<N;ii++){
        if(ii%(N/10)==0)cout << ".";

        float X = udist(rng)*xscale;
        float Y = udist(rng)*yscale;
        float Z = udist(rng)*ZMID;
        MY_VEC P = {X,Y,Z};

        //cout << X << "\t" << Y << "\t" << Z << "\n";

        if(is_Under_F(F_L,P) & is_Under_F(F_R,P) & is_Under_F(F_U,P) & is_Under_F(F_O,P)){//& is_Under_F(F_O,P)& is_Under_F(F_U,P) ){
            counter++;
            //cout << (int)X << "\t" << (int)Y << "\t" << (int)Z << "\n";
            }
        }
    cout << "\n" << "hit % = " << static_cast<float>(counter)/N * 100 << "\nBerechnete Dicke = " << static_cast<float>(counter)/N * ZMID << endl;

    std::cout << "Press ENTER to end... " << flush;
    //std::cin.clear();
    std::cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
    cin.get();



    return 0;
}
