#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Jofro


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs = {Up,Down,Left,Right};

  void search (const int& id) {
    Pos p = citizen(id).pos;
    bool money_nearby = false;
    Dir money_dir;

    for (Dir d : dirs) {
      Pos new_pos = p+d;
      if (pos_ok(new_pos) and cell(new_pos).bonus == Money) {
        money_nearby = true;
        money_dir = d;
      }

      else {
        if (d == Up) {
          for (Dir d2 : {Up, Right}) {
            Pos new_pos2 = new_pos + d2;
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }

        else if (d == Right) {
          for (Dir d2 : {Right, Down}) {
            Pos new_pos2 = new_pos + d2;
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }

        else if (d == Down) {
          for (Dir d2 : {Down, Left}) {
            Pos new_pos2 = new_pos + d2;
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }

        else {
          for (Dir d2 : {Left, Up}) {
            Pos new_pos2 = new_pos + d2;
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }
      }
  	}

    if (money_nearby) move(id, money_dir);
    else {
      Dir random_dir = dirs[random(0,3)];
	    if (pos_ok(p+random_dir) and (cell(p+random_dir).id == -1 or citizen(cell(p+random_dir).id).type == Builder)) move(id,random_dir);
    }
  }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    vector<int> b = builders(me());         // BUILDERS IDs
    vector<int> w = warriors(me());         // WARRIORS IDs
    /*
    if (is_day()) {                         // THE CURRENT ROUND IS DAY
      for (int id : b) {

      }
    }

    else {                                  // THE CURRENT ROUND IS NIGHT

    }*/
    for (int id : b) search(id);
    for (int id : w) search(id);
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
