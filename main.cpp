#include <iostream>
#include <ctime>
#include <stack>
#include <cstdlib>
#include <string>

using namespace std;

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
			score += (1 + tmp)*tmp / 2;
		}
		for (int i = 3; i<7; i++) {
			int tmp = sorted[i];
			score -= (1 + tmp)*tmp / 2;
		}
		score += cards[1] * 2;
		return score;
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

void converting(int &integer, char raw_input[], int &state_code, int player_num = 0) {
	if ((raw_input[0] == 'q' || raw_input[0] == 'Q') && raw_input[1] == 0) {
		state_code = 0;
		return;
	}
	else if ((raw_input[0] == 'R' || raw_input[0] == 'r') && (raw_input[1] == 'u' || raw_input[1] == 'U') && (raw_input[2] == 'l' || raw_input[2] == 'L') && (raw_input[3] == 'e' || raw_input[3] == 'E') && (raw_input[4] == 0)) {
		cout << "rule" << endl;
		return;
	}
	else if ((raw_input[0] == 'R' || raw_input[0] == 'r') && (raw_input[1] == 'e' || raw_input[1] == 'E') && (raw_input[2] == 'p' || raw_input[2] == 'P') && (raw_input[3] == 'l' || raw_input[3] == 'L') && (raw_input[4] == 'A' || raw_input[4] == 'a') && (raw_input[5] == 'y' || raw_input[5] == 'Y') && (raw_input[6] == 0)) {
		cout << "Undone replay feature" << endl;
		return;
	}
	else if ((raw_input[0] == 'h' || raw_input[0] == 'H') && raw_input[1] == 0) {
		cout << "Type a integer in the range of" << ((state_code == 1) ? " 3~5" : " ") << "or\n\"Rule\" to show rules\n\"Replay\" to restart a new game\n\"Q\" to quit the game" << endl;
	}
	else if ((raw_input[0]<'3' || raw_input[0]>'5') && raw_input[1] == 0 && state_code == 1) {
		cout << "The number of player should be in the range of 3~5" << endl;
		//state_code++;
		return;
	}
	else if ((raw_input[0] >= '3' || raw_input[0] <= '5') && raw_input[1] == 0 && state_code == 1) {
		integer = raw_input[0] - '0';
		state_code = 2;
		return;
	}
	else if ((raw_input[0] >= '1' && raw_input[0] <= '0' + player_num) && raw_input[1] == 0 && state_code == 4) {//for take deck
		integer = raw_input[0] - '1';
		return;
	}
	else if ((raw_input[0]<'1' || raw_input[0]>('0' + player_num )|| raw_input[1] != 0) && state_code == 4) {
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

void taking_cards(on_table_deck on_deck[], Player players[], int& state_code, int cur_player, int player_num) {
	char raw_input[20];
	int deck;
	bool is_all_taken = true;
	cout << "Which deck on the table do you prefer?";
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
			//cout<<'y';
			for (int i = 0; i<3 && on_deck[deck].read_card(i) != -1; i++) {
				//cout << on_deck[deck].read_card(i);
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
			//cout<<"state code"<<state_code<<"current player"<<cur_player;
			return;
		}
	}
}

void draw_card(on_table_deck on_deck[], int& state_code, stack<int>& main_deck, int cur_player, int player_num, bool& end_game, Player players[]) {
	//cout << main_deck.top();
	int tmp_card = main_deck.top();
	//cout << 1;
	int deck_num;
	char raw_input[20];
	if (main_deck.size() == 15) {
		end_game = true;
		cout << "The game will end after this round" << endl;
	}
	cout << "Player " << cur_player + 1 << " has drawn a ";
	print_card(tmp_card);
	cout << endl;
	for (int i = 0; i<player_num; i++) {
		cout << "Deck" << i + 1 << ": ";
		on_deck[i].print();
	}
	for (int i = 0; i<player_num; i++) {
		cout << "Player" << i + 1 << ':' << endl;
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
	int state_code = 1, player_num, cur_player = 0;
	char raw_input[20];
	stack<int> deck;
	Player players[5];
	on_table_deck on_deck[5];
	bool end_game;
	cout << "Welcome to Coloretto~\n\"H\" for help\n\"Rule\" to show rules\n\"Replay\" to restart a new game\n\"Q\" to quit the game" << endl;
	while (state_code != 0) {
		if (state_code == 1) {
			cout << "Please tell the host how many players are there?";
			cin >> raw_input;
			converting(player_num, raw_input, state_code);
			end_game = false;
		}
		if (state_code == 2) {
			for (int i = 0; i<player_num; i++) {
				cout << "Deck" << i + 1 << ": ";
				//bool tmp1=on_deck[i].isempty();
				//cout<<tmp1;
				on_deck[i].print();
			}
			int cnt[9] = { 3, 10, 9, 9, 9, 9, 9, 9, 9};
			srand(time(NULL));
			if (player_num>3) {
				for (int i = 0; i<player_num; i++) {
					int tmp = rand() % 7 + 2;
					cnt[tmp]--;
					players[i].add_card(tmp);
				}
			}
			else if (player_num == 3) {
				for (int i = 0; i<player_num; i++) {
					int tmp = rand() % 6 + 2;
					cnt[tmp]--;
					players[i].add_card(tmp);
				}
			}
			for (int i = 0; i<player_num; i++) {
				cout << "Player" << i + 1 << ':' << endl;
				players[i].print_card(player_num);
			}
			cout << endl;
			if (player_num>3) {
				for (int i = 0; i<76 - player_num; i++) {
					int tmp = rand() % 9;
					while (cnt[tmp] <= 0) {
						tmp = rand() % 9;
					}
					deck.push(tmp);
					cnt[tmp]--;
				}
			}
			else if (player_num == 3) {
				for (int i = 0; i<64; i++) {
					int tmp = rand() % 8;
					while (cnt[tmp] <= 0) {
						tmp = rand() % 8;
					}
					deck.push(tmp);
					cnt[tmp]--;
				}
			}
			/*int tmp = deck.size();
			for (int i = 0; i<tmp; i++) {
			cout << deck.top() << '\t';
			if ((i % 10) == 0)cout << endl;
			deck.pop();
			}*/
			state_code++;
		}
		if (state_code == 3) {//gaming stage start from here
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
				cout << "Time for player " << cur_player + 1 << endl;
				cout << "All decks on the table are full. You can only take a deck" << endl;
				state_code = 4;
			}
			else if (is_all_empty) {
				cout << "Time for player " << cur_player + 1 << endl;
				cout << "All decks on the table are empty. Draw a card." << endl;
				//cout << deck.top();
				state_code = 5;
			}
			else if (is_all_taken) {
				for (int i = 0; i<player_num; i++) on_deck[i].putback();
				state_code = 3;
			}
			else {
				cout << "Time for player " << cur_player + 1 << endl;
				cout << "What would you like to do?\nTake a deck with \'t\' key or draw a card with \'d\' key" << endl;
				cin >> raw_input;
				if ((raw_input[0] == 't' || raw_input[0] == 'T') && raw_input[1] == 0)
					state_code = 4;
				else if ((raw_input[0] == 'D' || raw_input[0] == 'd') && raw_input[1] == 0)
					state_code = 5;
				else {
					cout << "Please tell the host what do you want to do~" << endl;
				}
			}
		}
		if (state_code == 4) {//take stage
			taking_cards(on_deck, players, state_code, cur_player, player_num);
		}
		if (state_code == 5) {//draw stage
			draw_card(on_deck, state_code, deck, cur_player, player_num, end_game, players);
		}
		if (state_code == 6) {
			cur_player++;
			cur_player %= player_num;
			while (players[cur_player].is_over()) {
				cur_player++;
				cur_player %= player_num;
			}
			state_code = 3;
			/*int tmp = deck.size();
			for (int i = 0; i<tmp; i++) {
			cout << deck.top() << '\t';
			if ((i % 10) == 0)cout << endl;
			deck.pop();
			}*/
			cout << endl;
			for (int i = 0; i<player_num; i++) {
				cout << "Deck" << i + 1 << ": ";
				on_deck[i].print();
			}
			cout << "Scores:\t";
			for (int i = 0; i<player_num; i++) cout << "Player" << i + 1 << ": " << players[i].counting_score() << '\t';
			cout << endl << endl;
			for (int i = 0; i<player_num; i++) {
				cout << "Player" << i + 1 << ':' << endl;
				players[i].print_card(player_num);
			}
			cout << endl;
			//system("PAUSE");

		}
		if (state_code == 7) {
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
				cout << "Scores:" << endl;
				for (int i = 0; i<player_num; i++) cout << "Player" << i + 1 << ": " << players[i].counting_score() << '\t';
				cout << endl;
				for (int i = 0; i<player_num; i++) {
					cout << "Player" << i + 1 << ':' << endl;
					players[i].print_card(player_num);
				}
			}
		}
		if (state_code == 8) {
			cout << endl << "Would you like to play another round?" << endl;
			cin >> raw_input;
			if ((raw_input[0] == 'y' || raw_input[0] == 'Y')&&raw_input[1] == 0) state_code = 1;
			else if ((raw_input[0] == 'n' || raw_input[0] == 'N')&&raw_input[1] == 0) state_code = 0;
			else {
				cout << "Please enter Y or N to tell the host your will to restart the game" << endl;
				state_code = 8;
			}
		}
		switch (state_code) {
		case 41:
			cout << "The serial number of deck that you want to take is not in the range of 1~3" << endl;
			state_code = 4;
			break;
		case 42:
			cout << "What a pity~\nThe deck you want to take has been taken~" << endl;
			state_code = 4;
			break;
		case 43:
			cout << "The deck you want is empty~\nChoose another one please~" << endl;
			state_code = 4;
			break;
		case 51:
			cout << "Sorry, the range should be in 1~3" << endl;
			state_code = 5;
		case 52:
			cout << "The deck is full. Choose another one please~" << endl;
			state_code = 5;
			break;
		case 53:
			cout << "The deck is taken. Please choose another one~" << endl;
			state_code = 5;
			break;
		}
	}
	cout << "Thank you for playing~" << endl;
	return 0;
}
