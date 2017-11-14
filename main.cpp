#include <iostream>
#include <ctime>
#include <stack>
#include <cstdlib>
#include <vector>

using namespace std;

int tmp_state_code = 0;

void print_card(int);

class Player {
public:
	Player() {
		for (int i = 0; i<9; i++) cards[i] = 0;
		score = 0;
		isover = false;
	}
	void add_card(int i) {
		//cout<<i;
		cards[i]++;
		//cout<<cards[i];
		return;
	}
	void print_card(int player_num) {
		if (player_num>3) {
			cout << "Multi\t+2\tGray\tGreen\tBrown\tBlue\tPink\tYellow\tOrange" << endl;
			for (int i = 0; i<9; i++)
				cout << cards[i] << '\t';
		}
		else if (player_num == 3) {
			cout << "Multi\t+2\tGray\tGreen\tBrown\tBlue\tPink\tYellow" << endl;
			for (int i = 0; i<8; i++)
				cout << cards[i] << '\t';
		}
		cout << endl;
		return;
	}
	int counting_score() {
		int sorted[7];
		for (int i = 0; i<7; i++) sorted[i] = cards[i + 2];
		for (int i = 1; i<7; i++) {
			for (int j = i - 1; j >= 0; j--) {
				if (sorted[j + 1]>sorted[j]) {
					int tmp = sorted[j];
					sorted[j] = sorted[j + 1];
					sorted[j + 1] = tmp;
				}
			}
		}
		for (int i = 0; i<cards[0]; i++) {
			int j = 0;
			for (; sorted[j]>5; j++);
			sorted[j]++;
		}
		score = 0;
		for (int i = 0; i<3; i++) {
			int tmp = sorted[i];
			if(tmp<7)score += (1 + tmp)*tmp / 2;
			else score += 21;
		}
		for (int i = 3; i<7; i++) {
			int tmp = sorted[i];
			if (tmp<7)score -= (1 + tmp)*tmp / 2;
			else score -= 21;
		}
		score += cards[1] * 2;
		return score;
	}
	vector<int> all_cards() {
		vector<int> tmp(cards, cards + sizeof(cards) / sizeof(int));
		return tmp;
	}
	void set_finished() {
		isover = true;
	}
	void set_restart() {
		isover = false;
	}
	bool is_over() {
		return isover;
	}
	int most_card_num() {
		int tmp = 0;
		for (int i = 2; i<9; i++) if (cards[i]>tmp)tmp = cards[i];
		return tmp;
	}
	void clear() {
		for (int i = 0; i<9; i++) cards[i] = 0;
	}
private:
	int cards[9];
	int score;
	bool isover;
};

class on_table_deck {
public:
	on_table_deck() {
		for (int i = 0; i<3; i++) cards[i] = -1;
		is_taken = false;
	}
	bool isempty() {
		for (int i = 0; i<3; i++) {
			if (cards[i] != -1) return false;
		}
		return true;
	}
	bool isfull() {
		for (int i = 0; i<3; i++) {
			if (cards[i] == -1) return false;
		}
		return true;
	}
	bool istaken() {
		return is_taken;
	}
	void print() {
		for (int i = 0; i<3; i++) {
			print_card(cards[i]);
		}
		if (this->istaken())
			cout << "TAKEN";
		else if (this->isempty())
			cout << "empty";
		cout << endl;
	}
	void take() {
		for (int i = 0; i<3; i++) cards[i] = -1;
		is_taken = true;
	}
	void putback() {
		for (int i = 0; i<3; i++) cards[i] = -1;
		is_taken = false;
	}
	void putcard(int cur_card) {
		int tmp = 0;
		for (; cards[tmp] != -1; tmp++);
		cards[tmp] = cur_card;
	}
	int read_card(int i) {
		return cards[i];
	}
private:
	int cards[3];
	bool is_taken;
};

void AI_processing(Player players[], on_table_deck on_deck[], int cur_player, int player_num, int state, stack<int>& main_deck, int &state_code, bool &end_game) {
	vector<int> my_card = players[cur_player].all_cards();
	vector< vector <int> > oppo_card(player_num - 1, vector<int>());
	vector<int> take_deck_expectation(player_num, 0);
	bool is_all_taken = true;
	int take_expectation = 0, ai_state = state;
	for (int i = 0; i<player_num - 1; i++) oppo_card[i] = players[(cur_player + i) % player_num].all_cards();
	if (state == 1) {
		for (int i = 0; i<player_num; i++) {
			if (on_deck[i].istaken()||on_deck[i].isempty()) {
				take_deck_expectation[i] = -1;
			}
			else {
				for (int j = 0; j<3 && on_deck[i].read_card(j) != -1; j++) {
					if (on_deck[i].read_card(j) == 0) {
						take_deck_expectation[i] += (players[cur_player].most_card_num() + 1);
					}
					else if (on_deck[i].read_card(j) == 1) {
						take_deck_expectation[i] += 2;
					}
					else {
						take_deck_expectation[i] += my_card[on_deck[i].read_card(j)] + 1;
					}
					for (int k = 0; k<player_num - 1; k++) {
						take_deck_expectation[i] += oppo_card[k][on_deck[i].read_card(j)];
					}
				}
			}
		}
		int tmp = 0;
		for (int i = 0; i < player_num; i++) {
			if (take_expectation < take_deck_expectation[i]) {
				take_expectation = take_deck_expectation[i];
				tmp = i;
			}
		}
		if (take_expectation >= 3) {
			for (int i = 0; i<3 && on_deck[tmp].read_card(i) != -1; i++) {
				players[cur_player].add_card(on_deck[tmp].read_card(i));
			}
			players[cur_player].set_finished();
			on_deck[tmp].take();
			for (int i = 0; i<player_num; i++) {
				if (on_deck[i].istaken() == false) {
					is_all_taken = false;
					break;
				}
			}
			if (end_game ==true) {
				cout << "The game will end after this round" << endl << endl;
			}
			cout << "Player " << cur_player + 1 << " has taken deck " << tmp + 1 << "." << endl;
			system("PAUSE");
			state_code = is_all_taken ? 7 : 6;
			return;
		}
		else {
			srand(time(NULL));
			int tmp_card = main_deck.top(), deck_num = rand() % player_num;
			if (main_deck.size() <= 15) {
				end_game = true;
				cout << "The game will end after this round" << endl << endl;
			}
			while (on_deck[deck_num].istaken() || on_deck[deck_num].isfull()) {
				deck_num = rand() % player_num;
			}
			on_deck[deck_num].putcard(tmp_card);
			main_deck.pop();
			state_code = 6;
			cout << "Player " << cur_player + 1 << " has drawn ";
			print_card(tmp_card);
			cout<<endl<<endl<< "Player " << cur_player + 1 << " has put it to deck "<<deck_num+1;
			system("PAUSE");
			return;
		}
	}
	if (state == 2) {
		for (int i = 0; i<player_num; i++) {
			if (on_deck[i].istaken()||on_deck[i].isempty()) {
				take_deck_expectation[i] = -1;
			}
			else {
				for (int j = 0; j<3 && on_deck[i].read_card(j) != -1; j++) {
					if (on_deck[i].read_card(j) == 0) {
						take_deck_expectation[i] += (players[cur_player].most_card_num() + 1);
					}
					else if (on_deck[i].read_card(j) == 1) {
						take_deck_expectation[i] += 2;
					}
					else {
						take_deck_expectation[i] += my_card[on_deck[i].read_card(j)] + 1;
					}
					for (int k = 0; k<player_num - 1; k++) {
						take_deck_expectation[i] += oppo_card[k][on_deck[i].read_card(j)];
					}
				}
			}
		}
		int tmp = 0;
		for (int i = 0; i < player_num; i++) {
			if (take_expectation < take_deck_expectation[i]) {
				take_expectation = take_deck_expectation[i];
				tmp = i;
			}
		}
		for (int i = 0; i<3 && on_deck[tmp].read_card(i) != -1; i++) {
			players[cur_player].add_card(on_deck[tmp].read_card(i));
		}
		players[cur_player].set_finished();
		on_deck[tmp].take();
		for (int i = 0; i<player_num; i++) {
			if (on_deck[i].istaken() == false) {
				is_all_taken = false;
				break;
			}
		}
		if (end_game == true) {
			cout << "The game will end after this round" << endl << endl;
		}
		cout << "Player " << cur_player + 1 << " has taken deck " << tmp + 1 << "." << endl;
		system("PAUSE");
		state_code = is_all_taken ? 7 : 6;
		return;
	}
	else {
		srand(time(NULL));
		int tmp_card = main_deck.top(), deck_num=rand() % player_num;
		if (main_deck.size() <= 15) {
			end_game = true;
			cout << "The game will end after this round" << endl << endl;
		}
		while (on_deck[deck_num].istaken() || on_deck[deck_num].isfull()) {
			deck_num = rand() % player_num;
		}
		on_deck[deck_num].putcard(tmp_card);
		main_deck.pop();
		state_code = 6;
		cout << "Player " << cur_player + 1 << " has drawn ";
		print_card(tmp_card);
		cout << endl << endl << "Player " << cur_player + 1 << " has put it to deck " << deck_num + 1;
		system("PAUSE");
		return;
	}
}

void converting(int &integer, char raw_input[], int &state_code, int player_num = 0) {
	if ((raw_input[0] == 'q' || raw_input[0] == 'Q') && raw_input[1] == 0) {
		state_code = 0;
		return;
	}
	else if ((raw_input[0] == 'R' || raw_input[0] == 'r') && (raw_input[1] == 'u' || raw_input[1] == 'U') && (raw_input[2] == 'l' || raw_input[2] == 'L') && (raw_input[3] == 'e' || raw_input[3] == 'E') && (raw_input[4] == 0)) {
		cout << "You can choose either to draw a card, or take a deck on the table." << endl << "If you want to draw a card, then pick a deck which is not full or taken to rest the card." << endl
			<< "As for score counting, every the nth coloretto of the same color will be counted as n points. For example, 5 gray coloretto is counted as 1+2+3+4+5=15 points." << endl
			<< "Only the three colors with more number of colorettoes will be counted as plus point to the total score; other colors will be counted as minus points" << endl
			<< "A multi-color coloretto can be counted as any kind of coloretto, the system will automatically pick the one with highest point" << endl
			<< "Also, a +2 card can give you 2 points as the card shows." << endl << endl;
		system("PAUSE");
		system("CLS");
		return;
	}
	else if ((raw_input[0] == 'R' || raw_input[0] == 'r') && (raw_input[1] == 'e' || raw_input[1] == 'E') && (raw_input[2] == 'p' || raw_input[2] == 'P') && (raw_input[3] == 'l' || raw_input[3] == 'L') && (raw_input[4] == 'A' || raw_input[4] == 'a') && (raw_input[5] == 'y' || raw_input[5] == 'Y') && (raw_input[6] == 0)) {
		tmp_state_code = state_code;
		state_code = 9;
		return;
	}
	else if ((raw_input[0] == 'h' || raw_input[0] == 'H') && raw_input[1] == 0) {
		if (state_code == 1) {
			cout << "Type an integer in the range of 3~5 or\n";
		}
		else if (state_code == 4) {
			cout << "Type an integer in the range of 1~" << (char)('0' + player_num) << " or\n";
			state_code = 44;
		}
		else if (state_code == 5) {
			cout << "Type an integer in the range of 1~" << (char)('0' + player_num) << " or\n";
			state_code = 54;
		}
		cout << "Type \"Rule\" to show rules\n\"Replay\" to restart a new game\n\"Q\" to quit the game" << endl << endl;
	}
	else if ((raw_input[0]<'3' || raw_input[0]>'5') && raw_input[1] == 0 && state_code == 1) {
		cout << "The number of player should be in the range of 3~5" << endl;
		//state_code++;
		return;
	}
	else if ((raw_input[0] >= '3' || raw_input[0] <= '5') && raw_input[1] == 0 && state_code == 1) {
		integer = raw_input[0] - '0';
		state_code = 11;
		return;
	}
	else if ((raw_input[0] >= '1' && raw_input[0] <= '0' + player_num) && raw_input[1] == 0 && state_code == 4) {//for take deck
		integer = raw_input[0] - '1';
		return;
	}
	else if ((raw_input[0]<'1' || raw_input[0]>('0' + player_num) || raw_input[1] != 0) && state_code == 4) {
		state_code = 41;
		return;
	}
	else if ((raw_input[0] >= '1' && raw_input[0] <= '0' + player_num) && raw_input[1] == 0 && state_code == 5) {//for draw_card
		integer = raw_input[0] - '1';
		return;
	}
	else if ((raw_input[0]<'1' || raw_input[0]>'0' + player_num || raw_input[1] != 0) && state_code == 5) {
		state_code = 51;
		return;
	}
	else {
		cout << "Your input is invalid." << endl << "type \"H\" for help" << endl;
	}
}

void taking_cards(on_table_deck on_deck[], Player players[], int& state_code, int cur_player, int player_num, bool end_game) {
	char raw_input[20];
	int deck;
	bool is_all_taken = true;
	if (end_game) {
		cout << "The game will end after this round" << endl << endl;
	}
	cout << "Which deck on the table do you prefer?" << endl;
	cin >> raw_input;
	converting(deck, raw_input, state_code, player_num);
	if (state_code == 4) {
		if (on_deck[deck].istaken()) {
			state_code = 42;
			return;
		}
		else if (on_deck[deck].isempty()) {
			state_code = 43;
			return;
		}
		else {
			for (int i = 0; i<3 && on_deck[deck].read_card(i) != -1; i++) {
				players[cur_player].add_card(on_deck[deck].read_card(i));
			}
			players[cur_player].set_finished();
			on_deck[deck].take();
			for (int i = 0; i<player_num; i++) {
				if (on_deck[i].istaken() == false) {
					is_all_taken = false;
					break;
				}
			}
			state_code = is_all_taken ? 7 : 6;
			return;
		}
	}
}

void draw_card(on_table_deck on_deck[], int& state_code, stack<int>& main_deck, int cur_player, int player_num, bool& end_game, Player players[]) {
	int tmp_card = main_deck.top();
	int deck_num;
	char raw_input[20];
	system("cls");
	if (main_deck.size() <= 15) {
		end_game = true;
		cout << "The game will end after this round" << endl << endl;
	}
	cout << "Player " << cur_player + 1 << " has drawn a ";
	print_card(tmp_card);
	cout << endl << endl;
	for (int i = 0; i<player_num; i++) {
		cout << "Deck" << i + 1 << ": ";
		on_deck[i].print();
	}
	cout << endl;
	for (int i = 0; i<player_num; i++) {
		cout << ((players[i].is_over()) ? "*" : "") << "Player" << i + 1 << ':' << endl;
		players[i].print_card(player_num);
	}
	cout << endl;
	cout << "Which deck do you want to put it to?";
	cin >> raw_input;
	converting(deck_num, raw_input, state_code, player_num);
	if (state_code == 51) return;
	else if (state_code == 5) {
		if (on_deck[deck_num].isfull()) {
			state_code = 52;
			return;
		}
		else if (on_deck[deck_num].istaken()) {
			state_code = 53;
			return;
		}
		else {
			on_deck[deck_num].putcard(tmp_card);
			state_code = 6;
			main_deck.pop();
			return;
		}
	}
}

void print_card(int i) {
	switch (i) {
	case 0:
		cout << "Multi\t";
		break;
	case 1:
		cout << "+2\t";
		break;
	case 2:
		cout << "Gray\t";
		break;
	case 3:
		cout << "Green\t";
		break;
	case 4:
		cout << "Brown\t";
		break;
	case 5:
		cout << "Blue\t";
		break;
	case 6:
		cout << "Pink\t";
		break;
	case 7:
		cout << "Yellow\t";
		break;
	case 8:
		cout << "Orange\t";
		break;

	}
}

int main()
{
	int state_code = 1, player_num, cur_player = 0, AI_card_counting[9];
	char raw_input[20];
	stack<int> deck;
	Player players[5];
	on_table_deck on_deck[5];
	bool end_game, is_AI[5];
	cout << "Welcome to Coloretto~\n\"H\" for help\n\"Rule\" to show rules\n\"Replay\" to restart a new game\n\"Q\" to quit the game" << endl;
	while (state_code != 0) {
		if (state_code == 1) {
			cout << "Please tell the host how many players are there?";
			cin >> raw_input;
			converting(player_num, raw_input, state_code);
			end_game = false;
			for (int i = 0; i < 5; i++) {
				on_deck[i].putback();
				players[i].clear();
			}
		}
		if (state_code == 11) {
			cout << "Please tell the host how many AIs are there?";
			cin >> raw_input;
			while (raw_input[0] - '0'>player_num || raw_input[0]<'0' || raw_input[1] != 0) {
				cout << "Please enter a number in the range of 0~" << (char)(player_num + '0') << ", please~" << endl;
				cin >> raw_input;
			}
			for (int i = 0; i<raw_input[0] - '0'; i++) is_AI[i] = true;
			for (int i = raw_input[0] - '0'; i<player_num; i++) is_AI[i] = false;
			state_code = 2;
		}
		if (state_code == 2) {
			system("CLS");
			cout << "Initial state:" << endl;
			for (int i = 0; i<player_num; i++) {
				cout << "Deck" << i + 1 << ": ";
				on_deck[i].print();
			}
			int cnt_all[9] = { 3, 10, 9, 9, 9, 9, 9, 9, 9 }, cnt_draw[9] = { 0,0,1,1,1,1,1,1,1 };
			srand(time(NULL));
			if (player_num>3) {
				AI_card_counting[0] = 3; AI_card_counting[1] = 10; for (int i = 2; i<9; i++) AI_card_counting[i] = 9;
				for (int i = 0; i<player_num; i++) {
					int tmp = rand() % 7 + 2;
					while (cnt_draw[tmp] <= 0) {
						tmp = rand() % 7 + 2;
					}
					cnt_all[tmp]--;
					cnt_draw[tmp]--;
					AI_card_counting[tmp]--;
					players[i].add_card(tmp);
				}

			}
			else if (player_num == 3) {
				AI_card_counting[0] = 3; AI_card_counting[1] = 10; for (int i = 2; i<8; i++) AI_card_counting[i] = 9;
				for (int i = 0; i<player_num; i++) {
					int tmp = rand() % 6 + 2;
					while (cnt_draw[tmp] <= 0) {
						tmp = rand() % 6 + 2;
					}
					cnt_all[tmp]--;
					cnt_draw[tmp]--;
					AI_card_counting[tmp]--;
					players[i].add_card(tmp);
				}
			}
			for (int i = 0; i<player_num; i++) {
				cout << ((players[i].is_over()) ? "*" : "") << "Player" << i + 1 << ':' << endl;
				players[i].print_card(player_num);
			}
			cout << endl;
			if (player_num>3) {
				for (int i = 0; i<76 - player_num; i++) {
					int tmp = rand() % 9;
					while (cnt_all[tmp] <= 0) {
						tmp = rand() % 9;
					}
					deck.push(tmp);
					cnt_all[tmp]--;
				}
			}
			else if (player_num == 3) {
				for (int i = 0; i<64; i++) {
					int tmp = rand() % 8;
					while (cnt_all[tmp] <= 0) {
						tmp = rand() % 8;
					}
					deck.push(tmp);
					cnt_all[tmp]--;
				}
			}
			system("PAUSE");
			state_code++;
		}
		if (state_code == 3) {//gaming stage start from here
			system("CLS");
			bool is_all_full = true, is_all_empty = true, is_all_taken = true;
			for (int i = 0; i<player_num; i++) {
				if (on_deck[i].isfull() == false && on_deck[i].istaken() == false) {
					is_all_full = false;
					break;
				}
			}
			for (int i = 0; i<player_num; i++) {
				if (on_deck[i].isempty() == false) {
					is_all_empty = false;
					break;
				}
			}
			for (int i = 0; i<player_num; i++) {
				if (on_deck[i].istaken() == false) {
					is_all_taken = false;
					break;
				}
			}
			if (is_all_full) {
				system("CLS");
				cout << "Time for player " << cur_player + 1 << endl << endl;
				cout << "All decks on the table are full. You can only take a deck" << endl << endl;
				for (int i = 0; i<player_num; i++) {
					cout << "Deck" << i + 1 << ": ";
					on_deck[i].print();
				}
				cout << endl << endl;
				for (int i = 0; i<player_num; i++) {
					cout << ((players[i].is_over()) ? "*" : "") << "Player" << i + 1 << ':' << endl;
					players[i].print_card(player_num);
				}
				state_code = 4;
			}
			else if (is_all_empty) {
				system("CLS");
				cout << "Time for player " << cur_player + 1 << endl << endl;
				cout << "All decks on the table are empty. \nDrawing a card..." << endl << endl;
				system("PAUSE");
				system("CLS");
				state_code = 5;
			}
			else if (is_all_taken) {
				for (int i = 0; i<player_num; i++) on_deck[i].putback();
				state_code = 3;
			}
			else {
				if (is_AI[cur_player] == false) {
					system("CLS");
					cout << "Time for player " << cur_player + 1 << endl << endl;
					for (int i = 0; i<player_num; i++) {
						cout << "Deck" << i + 1 << ": ";
						on_deck[i].print();
					}
					cout << endl << endl;
					for (int i = 0; i<player_num; i++) {
						cout << ((players[i].is_over()) ? "*" : "") << "Player" << i + 1 << ':' << endl;
						players[i].print_card(player_num);
					}
					cout << "What would you like to do?\nTake a deck with \'t\' key or draw a card with \'d\' key" << endl;
					cin >> raw_input;
					if ((raw_input[0] == 't' || raw_input[0] == 'T') && raw_input[1] == 0)
						state_code = 4;
					else if ((raw_input[0] == 'D' || raw_input[0] == 'd') && raw_input[1] == 0) {
						system("cls");
						cout << "Time for player " << cur_player + 1 << endl << endl << "Drawing a card..." << endl;
						system("PAUSE");
						state_code = 5;
					}
					else {
						cout << "Please tell the host what do you want to do~" << endl;
						system("PAUSE");
					}
				}
				else {
					AI_processing(players, on_deck, cur_player, player_num, 1, deck, state_code, end_game);
				}
			}
		}
		if (state_code == 4) {//take stage
			if (is_AI[cur_player] == false) taking_cards(on_deck, players, state_code, cur_player, player_num, end_game);
			else {
				AI_processing(players, on_deck, cur_player, player_num, 2, deck, state_code, end_game);
			}
		}
		if (state_code == 5) {//draw stage
			if (is_AI[cur_player] == false) {
				draw_card(on_deck, state_code, deck, cur_player, player_num, end_game, players);
			}
			else {
				AI_processing(players, on_deck, cur_player, player_num, 3, deck, state_code, end_game);
			}
		}
		if (state_code == 6) {
			cur_player++;
			cur_player %= player_num;
			while (players[cur_player].is_over()) {
				cur_player++;
				cur_player %= player_num;
			}
			state_code = 3;
			cout << endl;
			system("CLS");
			cout << "State for now:" << endl << endl;
			for (int i = 0; i<player_num; i++) {
				cout << "Deck" << i + 1 << ": ";
				on_deck[i].print();
			}
			cout << endl << "Scores:\t";
			for (int i = 0; i<player_num; i++) cout << "Player" << i + 1 << ": " << players[i].counting_score() << '\t';
			cout << endl << endl;
			for (int i = 0; i<player_num; i++) {
				cout << ((players[i].is_over()) ? "*" : "") << "Player" << i + 1 << ':' << endl;
				players[i].print_card(player_num);
			}
			cout << endl;
			system("PAUSE");

		}
		if (state_code == 7) {
			system("CLS");
			if (end_game) {
				cout << "Here comes the result~:" << endl;
				for (int i = 0; i<player_num; i++) cout << "Player" << i + 1 << ": " << players[i].counting_score() << '\t';
				state_code = 8;
			}
			else {
				state_code = 3;
				for (int i = 0; i<player_num; i++) {
					players[i].set_restart();
					on_deck[i].putback();
				}
				cout << "A round has ended:" << endl << endl << "Scores:" << endl;
				for (int i = 0; i<player_num; i++) cout << "Player" << i + 1 << ": " << players[i].counting_score() << '\t';
				cout << endl << endl << "Cards:" << endl;
				for (int i = 0; i<player_num; i++) {
					cout << "Player" << i + 1 << ':' << endl;
					players[i].print_card(player_num);
				}
			}
			system("PAUSE");
		}
		if (state_code == 8) {
			system("CLS");
			cout << "Would you like to play another round?" << endl;
			cin >> raw_input;
			if ((raw_input[0] == 'y' || raw_input[0] == 'Y') && raw_input[1] == 0) state_code = 1;
			else if ((raw_input[0] == 'n' || raw_input[0] == 'N') && raw_input[1] == 0) state_code = 0;
			else {
				cout << "Please enter Y or N to tell the host your will to restart the game" << endl;
				state_code = 8;
			}
		}
		if (state_code == 9) {
			int tmp_cnt = 0;
			system("CLS");
			cout << "Player " << cur_player + 1 << " has requested to replay the game." << endl;
			for (; tmp_cnt<3; tmp_cnt++) {
				cout << "Is player " << (cur_player + 2 + tmp_cnt) % player_num << " willing to replay the game?";
				cin >> raw_input;
				while (raw_input[0] != 'Y'&&raw_input[0] != 'y'&&raw_input[0] != 'N'&&raw_input[0] != 'n'&&raw_input[1] != 0) {
					cout << "Type \'Y\' for yes, and \'N\' for no." << endl << "Tell the host your will~" << endl;
					cin >> raw_input;
				}
				if (raw_input[0] == 'N' || raw_input[0] == 'n') {
					cout << "Player " << (cur_player + 1 + tmp_cnt) % player_num + 1 << " has denied the request." << endl << "Back to the game." << endl;
					state_code = tmp_state_code;
					break;
				}
			}
			if (state_code == 9) {
				cout << "restarting..." << endl;
				state_code = 1;
				system("PAUSE");
				system("CLS");
			}
		}
		switch (state_code) {
		case 41:
			cout << "The serial number of deck that you want to take is not in the range of 1~" << player_num + 1 << endl;
			state_code = 4;
			break;
		case 42:
			cout << "What a pity~\nThe deck you want to take has been taken~" << endl;
			system("PAUSE");
			state_code = 4;
			break;
		case 43:
			cout << "The deck you want is empty~\nChoose another one please~" << endl << endl;
			state_code = 4;
			break;
		case 44:
			state_code = 4;
			system("PAUSE");
			break;
		case 51:
			cout << "Sorry, the range should be in 1~" << player_num << endl;
			system("PAUSE");
			system("CLS");
			state_code = 5;
			break;
		case 52:
			cout << "The deck is full. Choose another one please~" << endl;
			system("PAUSE");
			state_code = 5;
			break;
		case 53:
			cout << "The deck is taken. Please choose another one~" << endl;
			system("PAUSE");
			system("CLS");
			state_code = 5;
			break;
		case 54:
			state_code = 5;
			system("PAUSE");
			break;
		}
	}
	cout << "Thank you for playing~" << endl;
	system("PAUSE");
	return 0;
}
