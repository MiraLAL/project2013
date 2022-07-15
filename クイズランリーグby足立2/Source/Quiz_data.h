#pragma once
#include"Quiz.h"

Quiz_Data quizdata[numqes] =
{
    {{u8"「DARK SOULS」などで有名なFROM SOFTWAREの大体のゲームに登場する伝統的な武器の名前は次のうちどれ？"},{u8"Moonlight（月光)"},{u8"政宗(マサムネ)"},{u8"マスターソード"}      ,A,"./Data/Audio/テスト/ダークソール.wav"},
    {{u8"「星のカービィ」の主人公「カービィ」の身長は次のうちどれ？"},                                         {u8"10cmぐらい"},{u8"20cmぐらい"},{u8"30cmぐらい"}     ,B,"./Data/Audio/テスト/星のカービィ.wav"},
    {{u8"「大乱闘スマッシュブラザーズ」で出演していないキャラクターは？"},                                     {u8"ゴーゴート(ポケモン)"},{u8"豪鬼(ストリートファイター)"},{u8"フェリシア(ファイアーエムブレム)"}      ,C,"./Data/Audio/テスト/大乱闘 (mp3cut.net).wav"},
    {{u8"最も出荷本数が多い(売上が多い)のは?(2022/6/24時点、シリーズは含まない)"},                             {u8"Grand Theft Auto V(グランド　セフト　オート)"},{u8"Minecraft(マインクラフト)"},{u8"NieR Automata(ニーア　オートマタ)"}   ,B,"./Data/Audio/テスト/出荷本数.wav"},
    {{u8"次のうち原作で'本当に言っていない'のは？"},                                                           {u8"アムロ行きまーす(機動戦士ガンダム)"},{u8"とんでもないものを盗んでいきました(ルパン三世)"},{u8"オッス!おら悟空!(ドラゴンボール)"},C,"./Data/Audio/テスト/本当に言っていないの.wav"},
    {{u8"次のうち原作で'本当に言っている'のは？'"},                                                            {u8"ティッピ、うるさいです（ご注文はうさぎですか)"},{u8"なーちゃん、こいつ、今、あおった...!(アイドルマスター)"},{u8"チョコが一番ですわパクパクですわ（ウマ娘)"}     ,A,"./Data/Audio/テスト/本当に言っている.wav"},
    {{u8"弁当に入っている食品が混ざらないよう区切るための葉っぱのようなものの名前は次のうちどれ？"},           {u8"バラン"},{u8"花畑チャイカ"},{u8"バクラ"}          ,A,"./Data/Audio/テスト/弁当に入ってるやつ.wav"},
    {{u8"1575年織田信長が初めて銃を取り入れた織田と武田の戦いは次のうちどれ？"},                               {u8"おけはざまの戦い"},{u8"ながしのの戦い"},{u8"いしやまかっせん"}    ,B,"./Data/Audio/テスト/長篠の戦 (mp3cut.net).wav"},
    {{u8"夏の大三角形を構成する星で正しい組み合わせは次のうちどれ？'"},                                        {u8"アルタイル・ベガ・デネブ"},{u8"シリウス・プロキオン・ペテルギウス"},{u8"アンタレス・シリウス・デネブ"}     ,A,"./Data/Audio/テスト/夏の大三角形 (mp3cut.net).wav"},
    {{u8"次の動物の漢字はどれ？    たら'"},                                                                    {u8"白帯魚"},{u8"公魚"},{u8"大口魚"}     ,C,"./Data/Audio/テスト/動物漢字 (mp3cut.net).wav"},

};

Quiz_Data_Picture quiz_data[GENRE][numqes]
{
    {{"Data/Sprite/Quiz/問題/ota/ota_01.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_02.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_03.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_04.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_05.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_06.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_07.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_08.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_09.jpg"},
     {"Data/Sprite/Quiz/問題/ota/ota_10.jpg"},
    },

    {{}},
    {{}},
    
};
