#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <tuple>

// Class to represent a football club
class Club {
public:
    std::string name;
    int points;
    int matches_played;
    int matches_won;
    int matches_drawn;
    int matches_lost;
    int goals_scored;
    int goals_conceded;
    int quality_level;

    Club(const std::string& n, int q = 3) : name(n), points(0), matches_played(0),
        matches_won(0), matches_drawn(0), matches_lost(0), goals_scored(0),
        goals_conceded(0), quality_level(q) {}

    int getGoalDifference() const {
        return goals_scored - goals_conceded;
    }
};

// Comparison function for sorting clubs
bool compareClubs(const Club& a, const Club& b) {
    if (a.points != b.points) return a.points > b.points;
    if (a.getGoalDifference() != b.getGoalDifference()) return a.getGoalDifference() > b.getGoalDifference();
    if (a.goals_scored != b.goals_scored) return a.goals_scored > b.goals_scored;
    return a.name < b.name;
}

// Class to manage the league
class League {
private:
    std::vector<Club> clubs;

public:
    void addClub(const Club& club) {
        clubs.push_back(club);
        std::sort(clubs.begin(), clubs.end(), compareClubs);
    }

    void printRanking() const {
        std::cout << std::left << std::setw(20) << "Club" << std::setw(10) << "Points"
                  << std::setw(10) << "Played" << std::setw(10) << "Won"
                  << std::setw(10) << "Drawn" << std::setw(10) << "Lost"
                  << std::setw(10) << "GF" << std::setw(10) << "GA" << std::setw(10) << "GD" << std::endl;
        for (const auto& club : clubs) {
            std::cout << std::left << std::setw(20) << club.name
                      << std::setw(10) << club.points << std::setw(10) << club.matches_played
                      << std::setw(10) << club.matches_won << std::setw(10) << club.matches_drawn
                      << std::setw(10) << club.matches_lost << std::setw(10) << club.goals_scored
                      << std::setw(10) << club.goals_conceded << std::setw(10) << club.getGoalDifference() << std::endl;
        }
    }

    void addMatchResult(const std::string& home_team, const std::string& away_team, int home_goals, int away_goals) {
        auto home = std::find_if(clubs.begin(), clubs.end(), [&](const Club& c) { return c.name == home_team; });
        auto away = std::find_if(clubs.begin(), clubs.end(), [&](const Club& c) { return c.name == away_team; });

        if (home == clubs.end() || away == clubs.end()) {
            std::cout << "One or both teams not found in the league." << std::endl;
            return;
        }

        home->matches_played++;
        away->matches_played++;
        home->goals_scored += home_goals;
        home->goals_conceded += away_goals;
        away->goals_scored += away_goals;
        away->goals_conceded += home_goals;

        if (home_goals > away_goals) {
            home->points += 3;
            home->matches_won++;
            away->matches_lost++;
        } else if (home_goals < away_goals) {
            away->points += 3;
            away->matches_won++;
            home->matches_lost++;
        } else {
            home->points++;
            away->points++;
            home->matches_drawn++;
            away->matches_drawn++;
        }

        std::sort(clubs.begin(), clubs.end(), compareClubs);
    }

    void addMultipleMatchResults(const std::vector<std::tuple<std::string, std::string, int, int>>& results) {
        for (const auto& result : results) {
            addMatchResult(std::get<0>(result), std::get<1>(result), std::get<2>(result), std::get<3>(result));
        }
    }

    void simulateMatch(const std::string& home_team, const std::string& away_team) {
        auto home = std::find_if(clubs.begin(), clubs.end(), [&](const Club& c) { return c.name == home_team; });
        auto away = std::find_if(clubs.begin(), clubs.end(), [&](const Club& c) { return c.name == away_team; });

        if (home == clubs.end() || away == clubs.end()) {
            std::cout << "One or both teams not found in the league." << std::endl;
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);

        int home_goals = 0, away_goals = 0;
        for (int i = 0; i < 12; ++i) {
            if (dis(gen) < 0.05 * home->quality_level) home_goals++;
            if (dis(gen) < 0.05 * away->quality_level) away_goals++;
        }

        std::cout << home->name << " " << home_goals << " - " << away_goals << " " << away->name << std::endl;
        addMatchResult(home_team, away_team, home_goals, away_goals);
    }

    void simulateSpanishLeague() {
        for (int matchday = 1; matchday <= 38; ++matchday) {
            std::cout << "Matchday " << matchday << std::endl;
            for (size_t i = 0; i < clubs.size(); i += 2) {
                simulateMatch(clubs[i].name, clubs[i+1].name);
            }
            std::cout << "Current standings after matchday " << matchday << ":" << std::endl;
            printRanking();
            std::cout << std::endl;
        }
    }
};

int main() {
    League league;

    // Add some Spanish clubs
    league.addClub(Club("Real Madrid", 5));
    league.addClub(Club("Barcelona", 5));
    league.addClub(Club("Atletico Madrid", 4));
    league.addClub(Club("Sevilla", 4));
    league.addClub(Club("Valencia", 3));
    league.addClub(Club("Real Sociedad", 3));
    league.addClub(Club("Villarreal", 3));
    league.addClub(Club("Athletic Bilbao", 3));
    league.addClub(Club("Real Betis", 3));
    league.addClub(Club("Getafe", 2));
    league.addClub(Club("Celta Vigo", 2));
    league.addClub(Club("Levante", 2));
    league.addClub(Club("Granada", 2));
    league.addClub(Club("Eibar", 2));
    league.addClub(Club("Alaves", 2));
    league.addClub(Club("Real Valladolid", 2));
    league.addClub(Club("Osasuna", 2));
    league.addClub(Club("Cadiz", 1));
    league.addClub(Club("Elche", 1));
    league.addClub(Club("Huesca", 1));

    int choice;
    do {
        std::cout << "\n1. Print current ranking\n2. Add a match result\n3. Simulate a match\n"
                  << "4. Simulate entire Spanish League\n5. Add multiple match results\n0. Exit\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                league.printRanking();
                break;
            case 2: {
                std::string home, away;
                int home_goals, away_goals;
                std::cout << "Enter home team: ";
                std::cin >> home;
                std::cout << "Enter away team: ";
                std::cin >> away;
                std::cout << "Enter goals scored by " << home << ": ";
                std::cin >> home_goals;
                std::cout << "Enter goals scored by " << away << ": ";
                std::cin >> away_goals;
                league.addMatchResult(home, away, home_goals, away_goals);
                break;
            }
            case 3: {
                std::string home, away;
                std::cout << "Enter home team: ";
                std::cin >> home;
                std::cout << "Enter away team: ";
                std::cin >> away;
                league.simulateMatch(home, away);
                break;
            }
            case 4:
                league.simulateSpanishLeague();
                break;
            case 5: {
                int num_matches;
                std::cout << "Enter number of matches: ";
                std::cin >> num_matches;
                std::vector<std::tuple<std::string, std::string, int, int>> results;
                for (int i = 0; i < num_matches; ++i) {
                    std::string home, away;
                    int home_goals, away_goals;
                    std::cout << "Enter home team: ";
                    std::cin >> home;
                    std::cout << "Enter away team: ";
                    std::cin >> away;
                    std::cout << "Enter goals scored by " << home << ": ";
                    std::cin >> home_goals;
                    std::cout << "Enter goals scored by " << away << ": ";
                    std::cin >> away_goals;
                    results.emplace_back(home, away, home_goals, away_goals);
                }
                league.addMultipleMatchResults(results);
                break;
            }
            case 0:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}