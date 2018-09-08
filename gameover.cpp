#include "gameover.h"

vector<string> split(const string &s, char delim) {

    // splits the string into a vector then returns
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

vector <pair<int, string> > GameOver::getScores() {

    // make new vector
    vector < pair<int, string> > v;

    // read from scoreboard text file
    QFile file("scoreboard.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    // make a pair of names and scores, then push them onto score vector
    while (!in.atEnd()) {
        string line = in.readLine().toStdString();
        string name = split(line, ',')[0];
        int score = stoi(split(line, ',')[1]);
        v.push_back( make_pair(score, name) );
    }

    // close the file
    file.close();

    return v;
}

QString GameOver::scoresAsText(vector <pair<int, string> > scoreList) {

    // sort by scores
    sort(scoreList.begin(), scoreList.end());
    // reverse for descending order
    reverse(scoreList.begin(), scoreList.end());

    // format them into a printable QString
    stringstream ss;
    for (pair<int, string> p : scoreList) {
        ss << left << setw(8) << p.second << right << setw(3) << p.first << "\n";
    }
    cout << ss.str() << endl;
    // return this string
    return QString::fromStdString(ss.str());
}

GameOver::GameOver(QWidget* parent)
{

    // VARIABLE DECLARATIONS
    c = game::Config::getInstance();
    WIDTH = c->get_SCALEDWIDTH();
    HEIGHT = c->get_SCALEDHEIGHT();

    // BACKGROUND LABEL
    bg = new QLabel(parent);
    bg->setGeometry(QRect(0, 0, WIDTH, HEIGHT));
    bg->setVisible(false);
    bg->setStyleSheet("background-color: rgba(192, 57, 43, 150)");

    // LABEL FONTS
    QFont titleFont("IBM 3270 Semi-Narrow", 50, QFont::Bold);
    QFont scoreboardFont("Courier", 20);

    // TITLE LABEL
    int titleWidth = 400;
    int titleHeight = 100;
    title = new QLabel(parent);
    title->setVisible(false);
    title->setText("YOU LOSE");
    title->setGeometry(QRect(WIDTH/2-titleWidth/2, HEIGHT/2-titleHeight/2-100, titleWidth, titleHeight));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color:white; background-color: rgba(0,0,0,0)");
    title->setFont(titleFont);

    // SCOREBOARD LABEL
    int scoresWidth = 130;
    int scoresHeight = 200;
    scoreboard = new QLabel(parent);
    scoreboard->setVisible(false);
    scoreboard->setGeometry(QRect(WIDTH/2-scoresWidth/2, HEIGHT/2-scoresHeight/2+50, scoresWidth, scoresHeight));
    scoreboard->setAlignment(Qt::AlignLeft);
    scoreboard->setStyleSheet("color:white; background-color: rgba(0,0,0,0)");
    scoreboard->setFont(scoreboardFont);

}

void GameOver::appendScore(string playerName, int playerScore) {

    // open scoreboard file and add new player score to the end
    QFile file("scoreboard.txt");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream out(&file);

    out << QString::fromStdString(playerName) << "," << QString::number(playerScore) << "\n";
    file.close();

}


void GameOver::display(int playerScore) {

    // handle new player results and show the gameover screen
    string playerName = c->get_name().toStdString();
    appendScore(playerName, playerScore);
    vector< pair<int, string> > scores = getScores();
    scoreboard->setText(scoresAsText(scores));

    title->setVisible(true);
    bg->setVisible(true);
    scoreboard->setVisible(true);
}
