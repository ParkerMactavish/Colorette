#include <iostream>
#include <ctime>
#include <stack>
#include <cstdlib>
#include <string>

using namespace std;

class Player{
public:
    Player(){
        for(int i=0; i<9; i++) cards[i]=0;
    }
    void add_card(int i){
        cards[i]++;
        return;
    }
    void print_card(){
        cout<<"Gray\tGreen\tBrown\tBlue\tPink\tYellow\tOrange\tMulti\t+2"<<endl;
        for(int i=0; i<9; i++)
            cout<<cards[i]<<'\t';
        cout<<endl;
        return;
    }
private:
    int cards[9];
};

void printing_card(int* ontable_deck_i){
    int tmp_cnt=0;
    for(int i=0; i<3; i++){
        switch(*(ontable_deck_i+i)){
            case -1:
                tmp_cnt++;
                break;
            case 0:
                cout<< "Gray\t";
                break;
            case 1:
                cout<< "Green\t";
                break;
            case 2:
                cout<< "Brown\t";
                break;
            case 3:
                cout<< "Blue\t";
                break;
            case 4:
                cout<< "Pink\t";
                break;
            case 5:
                cout<< "Yellow\t";
                break;
            case 6:
                cout<< "Orange\t";
                break;
            case 7:
                cout<< "Multi\t";
                break;
            case 8:
                cout<< "+2\t";
                break;
        }
    }
    if(tmp_cnt==3)
        cout<<"empty";
}
void converting(int &integer, char raw_input[], int &state_code){
    if((raw_input[0]=='q'||raw_input[0]=='Q')&&raw_input[1]==0){
        state_code=0;
        return;
    }
    else if((raw_input[0]=='R'||raw_input[0]=='r')&&(raw_input[1]=='u'||raw_input[1]=='U')&&(raw_input[2]=='l'||raw_input[2]=='L')&&(raw_input[3]=='e'||raw_input[3]=='E')&&(raw_input[4]==0)){
        cout<<"rule"<<endl;
        return;
    }
    else if((raw_input[0]=='R'||raw_input[0]=='r')&&(raw_input[1]=='e'||raw_input[1]=='E')&&(raw_input[2]=='p'||raw_input[2]=='P')&&(raw_input[3]=='l'||raw_input[3]=='L')&&(raw_input[4]=='A'||raw_input[4]=='a')&&(raw_input[5]=='y'||raw_input[5]=='Y')&&(raw_input[6]==0)){
        cout<<"Undone replay feature"<<endl;
        return;
    }
    else if((raw_input[0]>='0'||raw_input[0]<='9')&&raw_input[1]==0){
        integer=raw_input[0]-'0';
        state_code++;
        return;
    }
    else{
        cout<<"Your input is invalid."<<endl<<"type \"Rule\" for rule"<<endl;
    }
}

int main()
{
    int state_code=1, player_num, ontable[3][3];
    char raw_input[10];
    stack<int> deck;
    Player players[5];
    cout<<"Welcome to Coloretto~\n\"H\" for help\n\"Rule\" to show rules\n\"Replay\" to restart a new game\n\"Q\" to quit the game"<<endl;
    while(state_code!=0){
        if(state_code==1){
            cout<<"Please the host how many players are there?";
            cin>>raw_input;
            converting(player_num, raw_input, state_code);
            if(player_num==4) state_code=2;
        }
        if(state_code==2){
            for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                    ontable[i][j]=-1;
            for(int i=0; i<3; i++){
                cout<<"Deck"<<i+1<<": ";
                printing_card(ontable[i]);
                cout<<endl;
            }
            int cnt[9]={9, 9, 9, 9, 9, 9, 9, 3, 10};
            srand(time(NULL));
            for(int i=0; i<4; i++){
                int tmp=rand()%7;
                players[i].add_card(tmp);
                cnt[tmp]--;
            }
            for(int i=0; i<4; i++){
                cout<<"player"<<i+1<<':'<<endl;
                players[i].print_card();
            }
            for(int i=0; i<72; i++){
                int tmp=rand()%9;
                while(cnt[tmp]==0){
                    tmp=rand()%9;
                }
                deck.push(tmp);
                cnt[tmp]--;
            }
            state_code++;
        }
        switch(state_code){
        case 1:
            cout<<"Number of players should be in the range of 3~5"<<endl;
            break;
        }
    }
    cout<<"Thank you for playing~"<<endl;
    return 0;
}
