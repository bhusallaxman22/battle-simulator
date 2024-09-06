/**
 * Advanced Battle Simulator
 * 
 * This program simulates a complex battle between two players with character classes,
 * special abilities, status effects, and a detailed battle system.
 * 
 * Compile: gcc -o advanced_battle_simulator advanced_battle_simulator.c -lm
 * Run: ./advanced_battle_simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_NAME 50
#define MAX_MOVES 5
#define MAX_CLASSES 4
#define MAX_STATUSES 5
#define MAX_INVENTORY 5

typedef enum {
    WARRIOR,
    MAGE,
    ROGUE,
    CLERIC
} CharacterClass;

typedef enum {
    PHYSICAL,
    MAGICAL,
    HEAL,
    BUFF,
    DEBUFF
} MoveType;

typedef enum {
    POISON,
    BURN,
    STUN,
    SHIELD,
    HASTE
} StatusEffect;

typedef struct {
    char name[20];
    int base_damage;
    int mana_cost;
    MoveType type;
    StatusEffect effect;
    float effect_chance;
} Move;

typedef struct {
    char name[MAX_NAME];
    CharacterClass class;
    int max_health;
    int current_health;
    int max_mana;
    int current_mana;
    int strength;
    int intelligence;
    int agility;
    Move moves[MAX_MOVES];
    StatusEffect statuses[MAX_STATUSES];
    int inventory[MAX_INVENTORY];
} Player;

// Function prototypes
void init_player(Player *player, const char *name, CharacterClass class);
void init_moves(Move moves[]);
void print_player_info(Player *player);
void print_moves(Player *player);
int get_player_move(Player *player);
void apply_move(Player *attacker, Player *defender, Move move);
void apply_status_effects(Player *player);
void use_item(Player *player);
void level_up(Player *player);
void battle(Player *player1, Player *player2);

// Global variables for moves and items
Move all_moves[20];
const char *item_names[] = {"Health Potion", "Mana Potion", "Strength Elixir", "Intelligence Elixir", "Agility Elixir"};

int main() {
    srand(time(NULL));
    init_moves(all_moves);

    Player player1, player2;
    char name[MAX_NAME];
    int class_choice;

    printf("Welcome to the Advanced Battle Simulator!\n\n");

    printf("Player 1, enter your name: ");
    scanf("%s", name);
    printf("Choose your class (0: Warrior, 1: Mage, 2: Rogue, 3: Cleric): ");
    scanf("%d", &class_choice);
    init_player(&player1, name, (CharacterClass)class_choice);

    printf("\nPlayer 2, enter your name: ");
    scanf("%s", name);
    printf("Choose your class (0: Warrior, 1: Mage, 2: Rogue, 3: Cleric): ");
    scanf("%d", &class_choice);
    init_player(&player2, name, (CharacterClass)class_choice);

    battle(&player1, &player2);

    return 0;
}

void init_player(Player *player, const char *name, CharacterClass class) {
    strncpy(player->name, name, MAX_NAME);
    player->class = class;

    switch (class) {
        case WARRIOR:
            player->max_health = 150;
            player->max_mana = 50;
            player->strength = 15;
            player->intelligence = 5;
            player->agility = 10;
            memcpy(player->moves, &all_moves[0], 5 * sizeof(Move));
            break;
        case MAGE:
            player->max_health = 100;
            player->max_mana = 150;
            player->strength = 5;
            player->intelligence = 20;
            player->agility = 8;
            memcpy(player->moves, &all_moves[5], 5 * sizeof(Move));
            break;
        case ROGUE:
            player->max_health = 120;
            player->max_mana = 80;
            player->strength = 12;
            player->intelligence = 8;
            player->agility = 18;
            memcpy(player->moves, &all_moves[10], 5 * sizeof(Move));
            break;
        case CLERIC:
            player->max_health = 130;
            player->max_mana = 120;
            player->strength = 8;
            player->intelligence = 15;
            player->agility = 7;
            memcpy(player->moves, &all_moves[15], 5 * sizeof(Move));
            break;
    }

    player->current_health = player->max_health;
    player->current_mana = player->max_mana;

    for (int i = 0; i < MAX_STATUSES; i++) {
        player->statuses[i] = -1;
    }

    for (int i = 0; i < MAX_INVENTORY; i++) {
        player->inventory[i] = i % 5;  // Initialize with some items
    }
}

void init_moves(Move moves[]) {
    // Warrior moves
    moves[0] = (Move){"Slash", 30, 0, PHYSICAL, -1, 0};
    moves[1] = (Move){"Cleave", 45, 20, PHYSICAL, -1, 0};
    moves[2] = (Move){"Shield Bash", 25, 15, PHYSICAL, STUN, 0.3};
    moves[3] = (Move){"Warcry", 0, 25, BUFF, -1, 0};
    moves[4] = (Move){"Berserk", 60, 40, PHYSICAL, -1, 0};

    // Mage moves
    moves[5] = (Move){"Fireball", 40, 30, MAGICAL, BURN, 0.4};
    moves[6] = (Move){"Ice Shard", 35, 25, MAGICAL, -1, 0};
    moves[7] = (Move){"Thunderbolt", 50, 40, MAGICAL, STUN, 0.2};
    moves[8] = (Move){"Arcane Shield", 0, 35, BUFF, SHIELD, 1.0};
    moves[9] = (Move){"Meteor", 70, 60, MAGICAL, BURN, 0.6};

    // Rogue moves
    moves[10] = (Move){"Backstab", 45, 20, PHYSICAL, -1, 0};
    moves[11] = (Move){"Poison Dart", 25, 15, PHYSICAL, POISON, 0.7};
    moves[12] = (Move){"Smoke Bomb", 0, 30, DEBUFF, -1, 0};
    moves[13] = (Move){"Swift Strike", 35, 25, PHYSICAL, HASTE, 0.5};
    moves[14] = (Move){"Shadow Dance", 55, 45, PHYSICAL, -1, 0};

    // Cleric moves
    moves[15] = (Move){"Smite", 35, 25, MAGICAL, -1, 0};
    moves[16] = (Move){"Heal", 40, 30, HEAL, -1, 0};
    moves[17] = (Move){"Purify", 0, 20, BUFF, -1, 0};
    moves[18] = (Move){"Holy Shield", 0, 35, BUFF, SHIELD, 1.0};
    moves[19] = (Move){"Divine Wrath", 60, 50, MAGICAL, -1, 0};
}

void print_player_info(Player *player) {
    printf("\n%s (%s)\n", player->name, 
           player->class == WARRIOR ? "Warrior" :
           player->class == MAGE ? "Mage" :
           player->class == ROGUE ? "Rogue" : "Cleric");
    printf("HP: %d/%d | MP: %d/%d\n", player->current_health, player->max_health, 
           player->current_mana, player->max_mana);
    printf("STR: %d | INT: %d | AGI: %d\n", player->strength, player->intelligence, player->agility);
    
    printf("Status Effects: ");
    for (int i = 0; i < MAX_STATUSES; i++) {
        if (player->statuses[i] != -1) {
            printf("%s ", 
                   player->statuses[i] == POISON ? "Poison" :
                   player->statuses[i] == BURN ? "Burn" :
                   player->statuses[i] == STUN ? "Stun" :
                   player->statuses[i] == SHIELD ? "Shield" : "Haste");
        }
    }
    printf("\n");
}

void print_moves(Player *player) {
    printf("Moves:\n");
    for (int i = 0; i < MAX_MOVES; i++) {
        printf("%d. %s (Damage: %d, Mana Cost: %d)\n", 
               i + 1, player->moves[i].name, player->moves[i].base_damage, player->moves[i].mana_cost);
    }
    printf("6. Use Item\n");
}

int get_player_move(Player *player) {
    int choice;
    do {
        printf("Enter your move (1-6): ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 6);
    return choice - 1;
}

void apply_move(Player *attacker, Player *defender, Move move) {
    if (move.mana_cost > attacker->current_mana) {
        printf("%s doesn't have enough mana to use %s!\n", attacker->name, move.name);
        return;
    }

    attacker->current_mana -= move.mana_cost;

    int damage = move.base_damage;
    if (move.type == PHYSICAL) {
        damage += attacker->strength;
    } else if (move.type == MAGICAL) {
        damage += attacker->intelligence;
    }

    // Apply agility for damage reduction
    damage = (int)(damage * (100.0 / (100 + defender->agility)));

    if (move.type == HEAL) {
        attacker->current_health = fmin(attacker->current_health + damage, attacker->max_health);
        printf("%s healed for %d HP!\n", attacker->name, damage);
    } else if (move.type != BUFF && move.type != DEBUFF) {
        defender->current_health -= damage;
        printf("%s dealt %d damage to %s!\n", attacker->name, damage, defender->name);
    }

    // Apply status effects
    if (move.effect != -1 && ((float)rand() / RAND_MAX) < move.effect_chance) {
        for (int i = 0; i < MAX_STATUSES; i++) {
            if (defender->statuses[i] == -1) {
                defender->statuses[i] = move.effect;
                printf("%s inflicted %s on %s!\n", attacker->name, 
                       move.effect == POISON ? "Poison" :
                       move.effect == BURN ? "Burn" :
                       move.effect == STUN ? "Stun" :
                       move.effect == SHIELD ? "Shield" : "Haste", 
                       defender->name);
                break;
            }
        }
    }
}

void apply_status_effects(Player *player) {
    for (int i = 0; i < MAX_STATUSES; i++) {
        switch (player->statuses[i]) {
            case POISON:
                player->current_health -= 10;
                printf("%s took 10 damage from Poison!\n", player->name);
                break;
            case BURN:
                player->current_health -= 15;
                printf("%s took 15 damage from Burn!\n", player->name);
                break;
            case STUN:
                printf("%s is Stunned and loses their turn!\n", player->name);
                break;
            case SHIELD:
                // Shield effect is handled in damage calculation
                break;
            case HASTE:
                // Haste effect could be implemented for extra actions
                break;
        }

        // 20% chance to remove the status effect each turn
        if (((float)rand() / RAND_MAX) < 0.2) {
            player->statuses[i] = -1;
        }
    }
}

void use_item(Player *player) {
    printf("Inventory:\n");
    for (int i = 0; i < MAX_INVENTORY; i++) {
        printf("%d. %s\n", i + 1, item_names[player->inventory[i]]);
    }

    int choice;
    printf("Choose an item to use (1-%d): ", MAX_INVENTORY);
    scanf("%d", &choice);
    choice--;

    switch (player->inventory[choice]) {
        case 0: // Health Potion
            player->current_health = fmin(player->current_health + 50, player->max_health);
            printf("%s used a Health Potion and recovered 50 HP!\n", player->name);
            break;
        case 1: // Mana Potion
            player->current_mana = fmin(player->current_mana + 50, player->max_mana);
            printf("%s used a Mana Potion and recovered 50 MP!\n", player->name);
            break;
        case 2: // Strength Elixir
            player->strength += 5;
            printf("%s used a Strength Elixir and gained 5 STR!\n", player->name);
            break;
        case 3: // Intelligence Elixir
            player->intelligence += 5;
            printf("%s used an Intelligence Elixir and gained 5 INT!\n", player->name);
            break;
        case 4: // Agility Elixir
            player->agility += 5;
            printf("%s used an Agility Elixir and gained 5 AGI!\n", player->name);
            break;
    }

    // Remove the used item
    player->inventory[choice] = -1;
}

void level_up(Player *player) {
    player->max_health += 20;
    player->max_mana += 10;
    player->current_health = player->max_health;
    player->current_mana = player->max_mana;

    switch (player->class) {
        case WARRIOR:
            player->strength += 3;
            player->intelligence += 1;
            player->agility += 2;
            break;
        case MAGE:
            player->strength += 1;
            player->intelligence += 4;
            player->agility += 1;
            break;
        case ROGUE:
            player->strength += 2;
            player->intelligence += 1;
player->agility += 3;
            break;
        case CLERIC:
            player->strength += 1;
            player->intelligence += 3;
            player->agility += 2;
            break;
    }

    printf("%s leveled up!\n", player->name);
    printf("Max HP +20, Max MP +10\n");
    printf("STR +%d, INT +%d, AGI +%d\n",
           player->class == WARRIOR ? 3 : player->class == MAGE ? 1 : player->class == ROGUE ? 2 : 1,
           player->class == WARRIOR ? 1 : player->class == MAGE ? 4 : player->class == ROGUE ? 1 : 3,
           player->class == WARRIOR ? 2 : player->class == MAGE ? 1 : player->class == ROGUE ? 3 : 2);
}

void battle(Player *player1, Player *player2) {
    int round = 1;
    while (player1->current_health > 0 && player2->current_health > 0) {
        printf("\n--- Round %d ---\n", round);

        // Player 1's turn
        print_player_info(player1);
        print_moves(player1);
        int move1 = get_player_move(player1);
        if (move1 == 5) {
            use_item(player1);
        } else {
            apply_move(player1, player2, player1->moves[move1]);
        }

        // Check if Player 2 is defeated
        if (player2->current_health <= 0) break;

        // Player 2's turn
        print_player_info(player2);
        print_moves(player2);
        int move2 = get_player_move(player2);
        if (move2 == 5) {
            use_item(player2);
        } else {
            apply_move(player2, player1, player2->moves[move2]);
        }

        // Apply status effects
        apply_status_effects(player1);
        apply_status_effects(player2);

        round++;

        // Level up every 5 rounds
        if (round % 5 == 0) {
            level_up(player1);
            level_up(player2);
        }
    }

    printf("\n--- Battle Ended ---\n");
    if (player1->current_health <= 0 && player2->current_health <= 0) {
        printf("It's a draw! Both players have been defeated.\n");
    } else if (player1->current_health > 0) {
        printf("%s wins!\n", player1->name);
    } else {
        printf("%s wins!\n", player2->name);
    }
}