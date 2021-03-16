#include <Siv3D.hpp>
using ll = int64;
#define rep(i, n) for (ll i = 0, _##i = (n); i < _##i; ++i)
#define elapsed (((double)Time::GetMillisec() - (double)StartTime) / 1000.0)
#define lane(i) ((ll)(i) % 4)

#define KeyA1 KeyA
#define KeyA2 KeyS
#define KeyA3 KeyD
#define KeyA4 KeyF
#define KeyB1 KeyZ
#define KeyB2 KeyX
#define KeyB3 KeyC
#define KeyB4 KeyV

const ColorF BackGroundColor = ColorF(0.8, 0.9, 1.0);
const int32 fps = 60;
const String Home =
    U"../src/";
double A_time = 0.016, C_time = 0.03, P_time = 0.07;

Key KEY(ll i)
{
    if (i == 0)
    {
        return KeyA1;
    }
    else if (i == 1)
    {
        return KeyA2;
    }
    else if (i == 2)
    {
        return KeyA3;
    }
    else if (i == 3)
    {
        return KeyA4;
    }
    else if (i == 4)
    {
        return KeyB1;
    }
    else if (i == 5)
    {
        return KeyB2;
    }
    else if (i == 6)
    {
        return KeyB3;
    }
    else if (i == 7)
    {
        return KeyB4;
    }
    return Key0;
}

inline double alpha() { return abs(Time::GetMillisec() % 1000 / 500.0 - 1); }
inline ColorF notecolor(int32 i)
{
    return (i < 4 ? ColorF(1.0, 0.0, 0.0, 0.5) : ColorF(0.0, 0.0, 1.0, 0.5));
}
inline ColorF linecolor(int32 i)
{
    return (i < 4 ? ColorF(1.0, 0.0, 0.0, 0.2) : ColorF(0.0, 0.0, 1.0, 0.2));
}

void Main()
{
    s3d::Scene::SetBackground(BackGroundColor);
    Graphics::SetTargetFrameRateHz(fps);
    String flag = U"Title";

    const Font TextNodeSize1(20);
    const Font TextNodeSize2(30);
    const Font TextNodeSize3(60);
    const Font TextNodeSize4(90);

    String song;
    Array<String> Songs;
    TextReader Reader(Home + U"songs.txt");

    while (Reader.readLine(song))
    {
        Songs.push_back(song);
    }

    const int32 SongNumInSelectMenu = 7;
    int32 SelectedNode = 0;
    int32 SelectedLevel = 0;

    double Volume = 0.5;

    String levels[] = {U"Easy", U"Hard", U"Master"};
    Color level_color[] = {Palette::Green, Palette::Blue, Palette::Red};

    int32 SelectedSong = 0;
    HashTable<String, Array<Vec3>> Charts[3];
    HashTable<String, double> Offsets, LevelNum[3], Length;
    HashTable<String, String> FileNames, BPMs;

    int32 A = 0, C = 0, P = 0, I = 0;
    String judge[] = {U"accurate", U"correct", U"permissible",
                      U"impermissible"};

    for (const auto &path :
         FileSystem::DirectoryContents(Home + U"songfiles/"))
    {
        if (FileSystem::Extension(path) == U"txt")
        {
            TextReader Reader(path);
            
            String SongName, FileName, BPM, Offset, line, length;

            Reader.readLine(SongName);

            Reader.readLine(FileName);
            FileNames[SongName] = FileName;

            Reader.readLine(BPM);
            BPMs[SongName] = BPM;

            Reader.readLine(Offset);
            Offsets[SongName] = Parse<double>(Offset);

            Reader.readLine(length);
            Length[SongName] = Parse<double>(length);

            ll level = 0;
            Array<Vec3> chart;
            while (Reader.readLine(line))
            {
                if (line.substr(0, 4) == U"Easy")
                {
                    if (chart.size())
                        Charts[level][SongName] = chart;
                    chart.clear();
                    level = 0;
                    LevelNum[level][SongName] =
                        Parse<double>(line.substr(5, line.size() - 5));
                    continue;
                }
                else if (line.substr(0, 4) == U"Hard")
                {
                    if (chart.size())
                        Charts[level][SongName] = chart;
                    chart.clear();
                    level = 1;
                    LevelNum[level][SongName] =
                        Parse<double>(line.substr(5, line.size() - 5));
                    continue;
                }
                else if (line.substr(0, 6) == U"Master")
                {
                    if (chart.size())
                        Charts[level][SongName] = chart;
                    chart.clear();
                    level = 2;
                    LevelNum[level][SongName] =
                        Parse<double>(line.substr(7, line.size() - 7));
                    continue;
                }
                else
                {
                    ll space = 0, keyid = 0;
                    double begin_time = 0, end_time = 0;
                    rep(i, line.size())
                    {
                        if (line[i] == ' ')
                        {
                            if (space == 0)
                            {
                                keyid = Parse<ll>(
                                    line.substr(space, i + 1 - space));
                            }
                            else
                            {
                                begin_time = Parse<double>(
                                    line.substr(space, i + 1 - space));
                            }
                            space = i + 1;
                        }
                    }
                    end_time =
                        Parse<double>(line.substr(space, line.size() - space));

                    chart.push_back(Vec3(keyid - 1, begin_time, end_time));
                }
            }
            if (chart.size())
                Charts[level][SongName] = chart;
            chart.clear();
        }
    }

    while (System::Update())
    {
        Audio BGM(Home + U"songfiles/");
        Audio Song(Home + U"songfiles/");

        Array<Vec3> Notes, notes;
        Array<bool> judged;
        ll StartTime = 0, speed = 10, default_offset = 5000;
        Array<RectF> Note, Light;
        Array<Color> color;

        String judge_status[8] = {U"", U"", U"", U"", U"", U"", U"", U""};
        double judge_alpha[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

        String Ranks[] = {U"D", U"C", U"B", U"A", U"S1", U"S2",
                          U"S3", U"S4", U"S5", U"S6", U"S7", U"S8"};

        while (System::Update())
        {

            if (flag == U"Title")
            {
                TextNodeSize4(U"RB PAD").drawAt(
                    Vec2(0, -100) + s3d::Scene::Center(), Palette::Red);
                TextNodeSize3(U"Press Any Key")
                    .drawAt(Vec2(0, 100) + s3d::Scene::Center(),
                            ColorF(1.0, 0.0, 1.0, alpha()));

                if (KeyA1.down() || KeyA2.down() || KeyA3.down() ||
                    KeyA4.down() || KeyB1.down() || KeyB2.down() ||
                    KeyB3.down() || KeyB4.down())
                {
                    flag = U"Menu";
                    SelectedNode = 0;
                    BGM = Audio(Home + U"songfiles/" +
                                    FileNames[Songs[SelectedNode]],
                                Arg::loop = true);
                    BGM.setVolume(Volume);
                    BGM.play();
                }
            }

            else if (flag == U"Menu")
            {
                TextNodeSize3(U"Select Menu")
                    .drawAt(Vec2(-100, -200) + s3d::Scene::Center(),
                            Palette::Red);
                TextNodeSize3(levels[SelectedLevel])
                    .drawAt(Vec2(250, -200) + s3d::Scene::Center(),
                            level_color[SelectedLevel]);
                TextNodeSize2(U"Volume: " + Format(Volume))
                    .drawAt(Vec2(250, 250) + s3d::Scene::Center(),
                            Palette::Red);
                TextNodeSize2(U"BPM: " + Format(BPMs[Songs[SelectedNode]]))
                    .drawAt(Vec2(-250, 250) + s3d::Scene::Center(),
                            Palette::Red);
                TextNodeSize2(
                    U"Level: " +
                    Format(LevelNum[SelectedLevel][Songs[SelectedNode]]))
                    .drawAt(Vec2(0, 250) + s3d::Scene::Center(), Palette::Red);
                if (Songs.size() <= SongNumInSelectMenu)
                {
                    rep(i, Songs.size())
                    {
                        TextNodeSize2(Songs[i]).drawAt(Vec2(0, -100 + 50 * i) +
                                                           s3d::Scene::Center(),
                                                       Palette::Red);
                        if (i == SelectedNode)
                        {
                            RectF(Arg::center = Vec2(0, -100 + 50 * i) +
                                                s3d::Scene::Center(),
                                  700, 45)
                                .draw(ColorF(1.0, 0.0, 1.0, alpha() / 3));
                        }
                    }
                }
                else
                {
                    rep(i, SongNumInSelectMenu)
                    {
                        ll index = (SelectedNode + i - SongNumInSelectMenu / 2 +
                                    Songs.size()) %
                                   Songs.size();
                        TextNodeSize2(Songs[index])
                            .drawAt(Vec2(0, -100 + 50 * i) +
                                        s3d::Scene::Center(),
                                    Palette::Red);
                        if (index == SelectedNode)
                        {
                            RectF(Arg::center = Vec2(0, -100 + 50 * i) +
                                                s3d::Scene::Center(),
                                  700, 45)
                                .draw(ColorF(1.0, 0.0, 1.0, alpha() / 3));
                        }
                    }
                }

                if (KeyA1.down())
                {
                    SelectedLevel += 1;
                    SelectedLevel %= 3;
                }

                if (KeyB1.down())
                {
                    SelectedLevel += 2;
                    SelectedLevel %= 3;
                }

                if (KeyA2.down())
                {
                    Volume = Max(0.0, Volume - 0.1);
                    BGM.setVolume(Volume);
                }

                if (KeyA3.down())
                {
                    Volume = Min(1.0, Volume + 0.1);
                    BGM.setVolume(Volume);
                }

                if (KeyB2.down())
                {
                    SelectedNode += Songs.size() - 1;
                    SelectedNode %= Songs.size();
                    BGM.stop();
                    BGM = Audio(Home + U"songfiles/" +
                                    FileNames[Songs[SelectedNode]],
                                Arg::loop = true);
                    BGM.play();
                }

                if (KeyB3.down())
                {
                    SelectedNode += 1;
                    SelectedNode %= Songs.size();
                    BGM.stop();
                    BGM = Audio(Home + U"songfiles/" +
                                    FileNames[Songs[SelectedNode]],
                                Arg::loop = true);
                    BGM.play();
                }

                if (KeyA4.down())
                {
                    flag = U"Title";
                    s3d::Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
                    BGM.stop();
                    SelectedNode = 0;
                }

                if (KeyB4.down())
                {
                    flag = U"Game";
                    s3d::Scene::SetBackground(Palette::White);
                    BGM.stop();
                    SelectedSong = SelectedNode;
                    SelectedNode = 0;
                    Notes = Charts[SelectedLevel][Songs[SelectedSong]];
                    Note.clear();
                    notes.clear();
                    judged.clear();
                    color.clear();
                    A = C = P = I = 0;
                    StartTime = Time::GetMillisec() + default_offset;
                    rep(i, 8)
                    {
                        Light.push_back(
                            RectF(Arg::center = Vec2(lane(i) * 150 - 225, 0) +
                                                s3d::Scene::Center(),
                                  140, 800));
                    }
                }
            }

            else if (flag == U"Game")
            {
                TextNodeSize1(levels[SelectedLevel])
                    .drawAt(Vec2(200, -170) + s3d::Scene::Center(),
                            level_color[SelectedLevel]);
                TextNodeSize1(U"BPM: " + Format(BPMs[Songs[SelectedSong]]))
                    .drawAt(Vec2(250, -210) + s3d::Scene::Center(),
                            Palette::Red);
                TextNodeSize1(
                    U"Level: " +
                    Format(LevelNum[SelectedLevel][Songs[SelectedSong]]))
                    .drawAt(Vec2(300, -170) + s3d::Scene::Center(),
                            Palette::Red);
                TextNodeSize1(Songs[SelectedSong])
                    .drawAt(Vec2(250, -250) + s3d::Scene::Center(),
                            Palette::Red);

                RectF(Arg::center = Vec2(0, 195) + s3d::Scene::Center(), 1700,
                      5)
                    .draw(Palette::Blue);

                RectF(Arg::center =
                          Vec2(-360 + 720 * elapsed /
                                          (Length[Songs[SelectedSong]] +
                                           Offsets[Songs[SelectedSong]]),
                               195) +
                          s3d::Scene::Center(),
                      10, 10)
                    .draw(Palette::Red);

                while (Notes.size() &&
                       elapsed + 600 / (speed * 60) >= Notes[0].y)
                {
                    if (Notes[0].y == Notes[0].z)
                    {
                        RectF note =
                            RectF(Arg::center =
                                      Vec2(lane(Notes[0].x) * 150 - 225, -400) +
                                      s3d::Scene::Center(),
                                  120, 10);
                        Note.push_back(note);
                        notes.push_back(Notes[0]);
                        color.push_back(notecolor(Notes[0].x));
                        judged.push_back(false);
                        Notes.pop_front();
                    }
                    else
                    {
                        ll length = (Notes[0].z - Notes[0].y) * 60 * speed;
                        RectF note = RectF(
                            Arg::center = Vec2(lane(Notes[0].x) * 150 - 225,
                                               -400 - length / 2) +
                                          s3d::Scene::Center(),
                            120, length + 10);
                        Note.push_back(note);
                        notes.push_back(Notes[0]);
                        color.push_back(notecolor(Notes[0].x));
                        judged.push_back(false);
                        Notes.pop_front();
                    }
                }

                rep(i, Note.size())
                {
                    Note[i].pos += Vec2(0, speed);
                    Note[i].draw(color[i]);
                }

                if (elapsed >= Offsets[Songs[SelectedSong]] &&
                    !Song.isPlaying())
                {
                    String FileName = FileNames[Songs[SelectedSong]];
                    Song = Audio(Home + U"songfiles/" + FileName);
                    Song.setVolume(Volume);
                    Song.play();
                }

                rep(i, 8)
                {
                    TextNodeSize1(judge_status[i])
                        .drawAt(Vec2(lane(i) * 150 - 225, 220) +
                                    s3d::Scene::Center(),
                                ColorF(0.0, 0.0, 0.0, judge_alpha[i]));
                    if (judge_alpha[i] > 0.5)
                        judge_alpha[i] -= 0.05;
                    else if (judge_alpha[i] > 0.0)
                        judge_alpha[i] -= 0.25;
                }

                if (elapsed >= -default_offset / 1000.0)
                {
                    if (KeyA1.pressed())
                    {
                        Light[0].draw(linecolor(0));
                    }

                    if (KeyA2.pressed())
                    {
                        Light[1].draw(linecolor(1));
                    }

                    if (KeyA3.pressed())
                    {
                        Light[2].draw(linecolor(2));
                    }

                    if (KeyA4.pressed())
                    {
                        Light[3].draw(linecolor(3));
                    }

                    if (KeyB1.pressed())
                    {
                        Light[4].draw(linecolor(4));
                    }

                    if (KeyB2.pressed())
                    {
                        Light[5].draw(linecolor(5));
                    }

                    if (KeyB3.pressed())
                    {
                        Light[6].draw(linecolor(6));
                    }

                    if (KeyB4.pressed())
                    {
                        Light[7].draw(linecolor(7));
                    }

                    rep(i, Note.size())
                    {
                        if (notes[i].y == notes[i].z)
                        {
                            if (elapsed >= notes[i].y + P_time)
                            {
                                judge_alpha[(ll)notes[i].x] = 1.0;
                                judge_status[(ll)notes[i].x] =
                                    U"Impermissible";
                                I++;
                                notes.remove_at(i);
                                Note.remove_at(i);
                                color.remove_at(i);
                                judged.remove_at(i);
                                _i--;
                                i--;
                                continue;
                            }
                            if (!KEY((ll)notes[i].x).down())
                                continue;
                            if (elapsed - A_time <= notes[i].y &&
                                notes[i].y <= elapsed + A_time)
                            {
                                judge_alpha[(ll)notes[i].x] = 1.0;
                                judge_status[(ll)notes[i].x] = U"Accurate";
                                A++;
                                notes.remove_at(i);
                                Note.remove_at(i);
                                color.remove_at(i);
                                judged.remove_at(i);
                                _i--;
                                i--;
                                continue;
                            }
                            else if (elapsed - C_time <= notes[i].y &&
                                     notes[i].y <= elapsed + C_time)
                            {
                                judge_alpha[(ll)notes[i].x] = 1.0;
                                judge_status[(ll)notes[i].x] = U"Correct";
                                C++;
                                notes.remove_at(i);
                                Note.remove_at(i);
                                color.remove_at(i);
                                judged.remove_at(i);
                                _i--;
                                i--;
                                continue;
                            }
                            else if (elapsed - P_time <= notes[i].y &&
                                     notes[i].y <= elapsed + P_time)
                            {
                                judge_alpha[(ll)notes[i].x] = 1.0;
                                judge_status[(ll)notes[i].x] = U"Permissible";
                                P++;
                                notes.remove_at(i);
                                Note.remove_at(i);
                                color.remove_at(i);
                                judged.remove_at(i);
                                _i--;
                                i--;
                                continue;
                            }
                        }
                        else
                        {
                            if (!judged[i] && KEY((ll)notes[i].x).down())
                            {
                                if (elapsed - A_time <= notes[i].y &&
                                    notes[i].y <= elapsed + A_time)
                                {
                                    judge_alpha[(ll)notes[i].x] = 1.0;
                                    judge_status[(ll)notes[i].x] = U"Accurate";
                                    judged[i] = true;
                                    A++;
                                    continue;
                                }
                                else if (elapsed - C_time <= notes[i].y &&
                                         notes[i].y <= elapsed + C_time)
                                {
                                    judge_alpha[(ll)notes[i].x] = 1.0;
                                    judge_status[(ll)notes[i].x] = U"Correct";
                                    judged[i] = true;
                                    C++;
                                    continue;
                                }
                                else if (elapsed - P_time <= notes[i].y &&
                                         notes[i].y <= elapsed + P_time)
                                {
                                    judge_alpha[(ll)notes[i].x] = 1.0;
                                    judge_status[(ll)notes[i].x] =
                                        U"Permissible";
                                    judged[i] = true;
                                    P++;
                                    continue;
                                }
                            }
                            if (judged[i] && elapsed > notes[i].z)
                            {
                                judge_alpha[(ll)notes[i].x] = 1.0;
                                judge_status[(ll)notes[i].x] = U"Accurate";
                                A++;
                                notes.remove_at(i);
                                Note.remove_at(i);
                                color.remove_at(i);
                                judged.remove_at(i);
                                _i--;
                                i--;
                                continue;
                            }
                            if (!KEY((ll)notes[i].x).pressed() &&
                                notes[i].y + P_time < elapsed &&
                                elapsed < notes[i].z - P_time)
                            {
                                judge_alpha[(ll)notes[i].x] = 1.0;
                                judge_status[(ll)notes[i].x] =
                                    U"Impermissible";
                                I += 1 + !judged[i];
                                notes.remove_at(i);
                                Note.remove_at(i);
                                color.remove_at(i);
                                judged.remove_at(i);
                                _i--;
                                i--;
                                continue;
                            }
                        }
                    }
                }

                if (KeyP.down() ||
                    elapsed >= Length[Songs[SelectedSong]] +
                                   Offsets[Songs[SelectedSong]])
                {
                    flag = U"Result";
                    s3d::Scene::SetBackground(ColorF(0.2, 0.9, 1.0));
                    Song.stop();
                    SelectedNode = 0;
                }
            }

            else if (flag == U"Result")
            {
                ll rank = 0, score = 0;

                score =
                    11111111 * (A * 1.0 + C * 0.9 + P * 0.5) / (A + C + P + I);

                score += 0.01;
                score = floor(score);

                if (score >= 8000000)
                {
                    rank++;
                }

                if (score >= 9000000)
                {
                    rank++;
                }

                if (score >= 9500000)
                {
                    rank++;
                }

                if (score >= 10000000)
                {
                    rank++;
                }

                if (score >= 11000000)
                {
                    rank++;
                }

                if (score >= 11100000)
                {
                    rank++;
                }

                if (score >= 11110000)
                {
                    rank++;
                }

                if (score >= 11111000)
                {
                    rank++;
                }

                if (score >= 11111100)
                {
                    rank++;
                }

                if (score >= 11111110)
                {
                    rank++;
                }

                if (score >= 11111111)
                {
                    rank++;
                }

                TextNodeSize3(U"Result").drawAt(
                    Vec2(0, -200) + s3d::Scene::Center(), Palette::Red);
                TextNodeSize3(U"Rank " + Ranks[rank])
                    .drawAt(Vec2(0, -125) + s3d::Scene::Center(), Palette::Red);
                TextNodeSize2(U"Score ", score)
                    .drawAt(Vec2(0, -50) + s3d::Scene::Center(),
                            Palette::White);
                TextNodeSize2(U"Accurate    ", A)
                    .drawAt(Vec2(0, 0) + s3d::Scene::Center(), Palette::White);
                TextNodeSize2(U"Correct    ", C)
                    .drawAt(Vec2(0, 50) + s3d::Scene::Center(), Palette::White);
                TextNodeSize2(U"Permissible    ", P)
                    .drawAt(Vec2(0, 100) + s3d::Scene::Center(),
                            Palette::White);
                TextNodeSize2(U"Impermissible    ", I)
                    .drawAt(Vec2(0, 150) + s3d::Scene::Center(),
                            Palette::White);
                TextNodeSize2(U"Notes ", A + C + P + I)
                    .drawAt(Vec2(0, 200) + s3d::Scene::Center(),
                            Palette::White);

                if (KeyB4.down())
                {
                    flag = U"Menu";
                    s3d::Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
                    SelectedNode = 0;
                }
            }
        }
    }
}
