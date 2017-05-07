//□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□
//
//   HEBORIS [ヘボリス] 2001 Ver1.60β1 / 2000.11.05 (Sun) ～ 2001.07.29 (Sun)
//
//                         (c) 1998-2001 Kenji Hoshimoto
//
//   Version 1.60 (02/03/31) 横移動を修正、フィールドの背景を消せるように
//                           ブロックが突然消えるバグを修正
//   Version 1.51 (01/12/04) 落ちてしまうバグを修正
//   Version 1.50 (01/11/26) ランキング機能追加 (インターネットランキング対応)
//                           リプレイ・ポース・NEXT隠し機能追加 デモ画面追加
//                           全消しボーナス追加 デビルモードをもう少し簡単に
//                           その他いっぱい
//
//■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■
/*
C7V4EX YGS2K+
・NEXTブロック表示の更新が、ブロック出現の瞬間に1フレ遅れていた問題を修正（先行HOLD時のHOLDブロックも）
・始めから入っているミッションデータは、debug=1でないと編集できないようにした。
・コンパイル中、画面上部に表示する文字を変更
・SETTING > NUMFONTがSHARPの時の、大きい数字フォントのデザインを変更

C7V3.8EX YGS2K+
・コンパイル中に画像を表示できるYGS2k改造版を標準装備に。
　コンパイル開始時は \SE\welcome.wav が鳴ります。（compiling.iniで変更可）
・IRS無効設定で撮ったリプレイの詳細表示では
　IRS STYLE を IRS DISABLED と表示するように変更

・MAX PLAYER が SINGLE のとき、SETTING >> INPUT > INPUT TEST で2Pのキーボード入力が
　認識されなかった問題を修正
・その他見栄えで色々

C7V3.75EX YGS2K
・アイテム「REFLECT」に追加効果。
　発動の瞬間、ステータス異常をクリアします。
　1人用アイテムモードではフィールド枠の色が変わり、効果時間が3倍になり、攻撃アイテムの効果を
　一度だけ防ぎます。（防ぐと「GUARD!」と表示される）
・アイテム「FEVER!」で攻撃アイテムを防いだ時も「GUARD!」と表示するように変更。
・アイテム「180°FIELD」発動中に生じる間を調整

C7V3.7EX YGS2K
・アイテムなどによるブロック強制消去エフェクトを変更
・HOLD(非IHS)処理タイミング変更。再び先頭へ。
　もう勝手に先行移動しないお（＾ω＾）
・アイテム「16t」の演出を変更
・その他見栄えで色々

C7V3.65EX YGS2K
・DEVILモードにITEM MODEを解禁。DEATH+との併用は出来ません。
　一部のアイテムは死亡フラグ？
・ウインドウタイトルをコンパイル中＆データ読み込み中に変えるようにした。
・一部のアイテムによる演出時の間をAREに応じて変えるようにした。
　（すでにAREが作用しているものや、プレイヤー間の同期が必要なものを除く）
・自然落下時のブロック残像を、「常に」ではなく「1ブロック分以上落下した時」に変更

C7V3.6EX YGS2K
・DEVIL-DOOM速度調整
　どうみても300-499間が速すぎでした。ほんとうに(ry
・アイテム「スカ」に追加効果をつけた。
　レベル上昇なし、スコア・ライン数加算なしになります。　アッー！
・アイテム発動警告エフェクトを強化
　半透明処理を使っていますが、処理落ち時は擬似半透明（点滅）に切り替わります。
・MISSION「デビルロード」のOOBAKAノルマ数を6に変更。
　7ブロックだった頃、バグにより開始前にノルマが1つカウントされていたため、実質6ブロックだった。

C7V3.55EX YGS2K
・VSモードの速度設定に、PRACTICE等と同じfavoritsが使えるようになりました。
　それに伴い、OOBAKAは隠し速度にしました。　左入力しつづけると…？
・リプレイでの早送り中、早送り倍率を表示するようにした。
・高解像度時のデモに飾りを追加
・VSのgoal設定がレベルの場合、レベルストップはgoalLvの手前のみにした。
・http://www.tetrisconcept.com/forum/viewtopic.php?t=576　で指摘されていた
　「効果音が1フレ早い」問題は、LITEだけでなくC7系にも言えるのでLITEに合わせた。

・D.R.Sで20Gの時、ブロック固定後、ARE時に下入れし直すと、1ブロック即固定と同時に
　次のブロックまで出現していた問題を修正。
・TLSの擬似立体部分が明るすぎた問題を修正

C7V3.5EX YGS2K
・即落下と同時にリプレイの記録が終了した場合、リプレイで最後の動作（＝即落下）が
　再現されない問題を修正。
・MISSIONのOOBAKAでノルマが1ブロックにつき2カウントされていた問題を修正
　同モードのスクウェアクリア時にラインを消さない場合、勝手に1ラインせり上がる事があった問題を修正
・コンボ表示もアイテム「TIME STOP」で時間停止するように。
・TAP/TI式ブロック消去エフェクトのアニメーションを2倍速にするする基準を調整
・ミッションで、BGM19(tomoyo_eh_final)が使えなかった問題を修正

C7V3.45EX YGS2K
・DEVIL-DOOM速度調整。
　AREは長く、固定と横溜めが短くなりました。序盤での飛ばしがかなりやりやすく。
・MASTER・20G-#G4のquality cool(速度cool)を出やすくしました。
　また、SP:rank110以降は加速しないようにしました。

・showcombosの有無に関わらず、必ずコンボ表示が出ていた問題を修正。

C7V3.4EX YGS2K
・SRS-Xのブロック柄を変更しました。
・SHOW COMBOS大幅強化。3コーナーT-spinやBack to Back も表示できます。
　また、表示されるコンボ数は「1ライン消しを含まないコンボ」に変更
・画面リフレッシュのタイミングを「内部処理の前」に変更（LITEと同じ）
・ARE + 横溜めの合計値が一定値以下の場合、TAP/TI式ブロック消去エフェクトのアニメーション速度を2倍速にしました。
　（超高速下での視認性の関係で）
・Tiのプラチナブロックをよ～く見てると、微妙に光るアニメーションが確認できたので微妙に光らせてみた。
　ついでにアイテムブロックも微妙に光らせてみた。
・ブロック出現・IRS・IHSの瞬間に発生する余分な1フレームを短縮
・プレイ中はNEXT表示数を変えられないようにした。（VSでのアイテムブロック出現位置の関係で）

・レベルをカンストさせ、ロール突入前のブロックが消えていく演出中に、横溜めを溜められない問題を修正

C7V3.3EX YGS2K
・TOMOYO ACE-TGTとミッションのターゲットでプラチナブロックの配置をランダムにしました。
　1列につき1個配置されます。
　個数はミッションではopt値で指定します(99にすると…？)。TOMOYO-TGTではステージを進めるにつれて数が多くなっていきます。
・ACE-TGTのステージを少し追加

・リプレイ選択画面でVSリプレイを選択した状態でメニューに戻ると、
　SOLOモードでもgameModeがVSのままだった問題を修正
・[buglist掲載バグ]TOMOYO ACE-TGTのステージ21に到達すると、勝手にIRSがACE+式になる問題を修正
　原因はrecStageTime関数での配列の範囲を超えた指定によるもの

C7V3.2EX YGS2K
・VSデモで決着が着くと、デモなのにリプレイ保存メニューが出る問題を修正
・PRACTICEで終了条件を設定した場合、終了時にブロックを消さないように
・オールドスタイル時は残像を無効に
・ORIGINALモードに15分モード追加

C7V3.1EX YGS2K
・heboris.iniに、BGMフェードアウト速度を設定できる機能を追加（fade_seed）
・ロード中に表示する画像を調整
・3コーナーT-spin消しをしたときの表示が、特定条件下で変になるのを修正。
　例えば、DT砲等でT-spinダブル・トリプルを行うとトリプルがダブルと表示される等

C7V3.0EX VS REPLAY AVAILABLE
・VSリプレイのずれを修正　…できたと信じたい(｀・ω・´)
　（VSモード開始のタイミングが、プレイ時とリプレイ時でプレイヤー間に1フレの誤差があった（→ステータスNo.10））
・VSラウンド間でBIGモードが維持されなかった問題を修正
・hebosprをpng化してから、どうも拡大縮小描画時に透過色が上手く抜けていなかったので、透過する部分の色を変更

C7V2.9EX VS REPLAY AVAILABLE
・VSモードのリプレイが記録できるようになりました。
　1つのファイルに2人分のデータを記録します。
　リプレイ時間は10分までです。
　CPU vs CPU　のプレイは記録できません。
　heboris.iniの「item_pro」を変更すると、アイテム使用時に、ずれます。
・VSがポイントマッチ制になりました。VSメニューでHOLD+上下操作で勝利ポイントを変更できます。
・ブロック固定の瞬間に発生する余分な1フレームをさらに短縮（LITEの「SKIP LOCKFLASH:SUPER」に相当）
　D.R.SでAREスキップ可能時は短縮しません（勝手にAREスキップが発動してしまうため）
・リプレイを40個まで保存できるようにしました。
・リプレイ保存メニューを追加　リプレイが存在する場合、ある程度の情報が表示されます
　両プレイヤーとも、1～40番にリプレイを保存できます。
・ブロック枠外出現時のNEXT遅延を調整
・実際にプレイした体感速度を元にDOOMロール速度を調整
・block_spectrum = 1　の時、横移動でも残像を発するように。　液晶モニタだとあまり意味は無い？

・SIMPLE-40L・2minのリプレイで、速度が勝手に20Gになる（＝非20Gで撮ったリプがずれる）問題を修正。

C7V2.43EX YGS2K
・ロード中に表示する画像を変更　ランダムで変化します
・ACE-NORMALとミッションの通常レベルの速度を遅めにした
　（ただしデビルロードは旧速度を使用）
・ROTリレーで1回転法則ごとに背景を変えるように
・EXCELLENT表示時に「TRY AGAIN TO…」が一瞬しか表示されない問題を修正
・オールドスタイルのARCADEバージョンでBIGにするとブロックのグラフィックが変化しなかった問題を修正
・同モードでBIGブロックが天井をスルーする問題も修正

（バージョン変更無し）
・ハードブロックを消した時、ハードブロックよりも下の消したライン分が
　落下しなかった問題を修正
・操作ブロックがハードブロックの時にラインを消すと、アイテムが発動しなかった問題を修正

C7V2.42EX YGS2K
・CPUにロールロールに対するリアクションを追加。回転入力を行わなくなります。
・「nmlroll_bgmloop」が0だと、「bgnroll_bgmloop」の設定が無視される問題を修正。
・ハードブロックよりも右側にあるブロックの消去エフェクトが出ていなかった問題を修正。
・SIMPLE＞スクウェアモードがリプレイで再現されない可能性があったのを多分修正
・対戦で、DS-RANDOMせり上がりに関して、ATTACKの穴保持率を4割に変更。
・スクウェアで3コーナーT-SPINを行った時、フリーフォールと共に全ブロックを破片化するように。
・BIG用初期化（BigInit）はとっくに不要になっていたので削除
・ミッション「BIG」の次で音楽が変わる場合に落ちる問題を再度修正

C7V2.41EX YGS2K
・対戦で、相手がDS-RANDOMせり上がりの時の穴保持率をスタイルごとに変えるようにした。
　（NORMAL 6割、ATTACK 3割、DEFENCE & ITEM、7割）
・GAMEOVER表示後の背景フェードに関する問題を修正
・アイテムブロック消去時、操作中のブロックがアイテムならば、それを発動するように
・DS-RANDOMせり上がりの穴保持率に1割分の間違いがあったのを修正

C7V2.4EX YGS2K
・スタッフロール（BEGINNER以外）に無限回転対策を追加
　bgmをタイミングよく終わらせるためにも、無限回転は避けましょう
・対戦に「スタイル」を追加。詳しくは「\doc\versus.txt」を参照
・対戦では、アイテム「FREE FALL」で消えたラインを相手に送り込めるようにした。
　ただし、相手のせり上がり形式が「MOVE ERASED」だと穴が1列に揃っているので
　Back to Backヘボリス等で返されないように…
・PRACTICEで高速ロールがシュミレーション出来るようになりました。
・PRACTICEで捨てゲーしたときも死亡時同様にライン情報を表示するように
・足切りされたとき、ミッション以外はラインを消した瞬間に足切り演出を開始するようにした。
・足切り時の「But... Let's…」表示時に花火を消すようにした。　　TIっぽく
・リプレイ保存に速度制限のテスト。設定値より実FPSが10FPS以上差があると保存できなくしてみる。
・hebosprをpng化＆調整
・高解像度時の「GAME OVER」表示に「あの」メッセージを追加

C7V2.3EX ROT.RELAY MODE DEBUT
・SIMPLEにROT.RELAY追加。　リプレイ時間の関係上40LINESとULTRA2minのみ。
　全ての回転法則で40Lか2MINに挑戦します。
　窒息してもゲームオーバーにはなりませんが、ペナルティがあります。
　速度は最初にしか選べません。
　結果表示ではベスト記録とワースト記録が強調表示されます。
・SETTING > DESIGNの「FONT SIZE」を「NUMBER FONT」に変更
　SHARPにするとsmall & BIGフォントの数字が変化します。
・「quickerase」の項目を廃止。代わりにSETTING > DESIGNで「top_frame」を変更可能に
　ORIGINALはquickerase:OFFの時の、他はONの時の演出になります。
・ORIGINALモードのスコア計算タイミングをオリジナル版に合わせました。
　また、同モードのブロック消去演出はTGM破壊エフェクトに固定しました。
・シングル台でミッションのリプレイを再生した時、飾りがずれていたのを修正
・過去のMASTER #2 & #G3のリプレイ互換性を修正

C7V2.28EX YGS2K
・飾りをいろいろと追加
・OPTION画面でT-SPINタイプが2までしか選べない問題を修正
・T-SPINタイプ3のとき花火ボーナスがTi式T-SPINの場合にしか加算されない問題を修正
・REPLAYのときに表示されるLINE INFOのカウントに関する問題を修正

C7V2.25EX YGS2K
・SOLO MODE回転法則選択画面の手入れ
・ネームエントリー時、マスターor20Gの #G4では段位を表示するようにしました。
・ネームエントリー時、裏段位が成立していれば表示するようにしました。
・低解像度時のスタッフロールも画像にしました。
・ミッションのイレイサー最終ラインが他のラインと重なる事があった問題を修正

C7V2.2EX YGSK
・VSのアイテムセットに「TGM」を追加
・低解像度時のグラフィックを整備
・アイテム「REFLECT」発動中はフィールド色を変えないようにしました。
　こっそり発動して相手を痛い目にあわせましょう。
・C7V2.1でCPUに削りを覚えさせたつもりが、結構バカな位置に置くようになったので削りは忘れさせた。
・古いバージョンのリプレイを見た後、アイテムの総数がリプレイ収録当時のままになる問題を修正
・低解像度時にsmallフォントの数字とbigフォントの透過色が正しく設定されていなかった問題を修正
・アイテム(主にTGM系)出現率の見直し
　http://blogs.yahoo.co.jp/hr_tgm_pinfu/archive/2007/02/23　←参考ページ

C7V2.1EX YGS2K
・ブロックが固定されて、ステータス値が変更されたと同時に次のステータス関数を実行する事で余分な間を1フレ短縮
　（LITEのSKIP LOCKFLASH：ONと同じくらい）　DOOMがよりそれっぽく
・スタッフロール中に発生するAREの余分な1フレームを短縮
・DEFAULTフォントによるスコア表示を廃止。
　スコア表示のレイアウトをモードごとに変更
・メダルの配置を変更　BEGINNER～DEVILモードにLINES表示復活
・heboris.iniの各メダル関連の情報の表示の有無を変える「hide_skill、hide_allclear、hide_combo、hide_rescue、hide_ST」を
　「medal_info」に統一
・リプレイ中にライン消去情報を表示できるようにしました。邪魔な時はHOLDで隠せます。
・文字描画関数に「PrintTinyFont」を追加。1文字6x7(低解像度時)と非常に小さい
　いまのところ、数字、/ : < > 、アルファベット大文字が使用可能　ただし字詰め無し
・アイテム「GRANDMOTHER BLOCK」を追加。全てのブロック（アイテムブロック含む）が[ ]になります。
　DS-WORLDでも1色になります。
・アイテム「スカ」をホールドしても消えないように。
　また、スカ効果中はブロックを置くたびにフィールドがプルプルするように
・speed.def更新（MASTER後半部分）
・ミッションのイレイサーで、最後のラインは最上段に出すように。
・DSロード、デビルロード、ヘボマニア、プラチナを調整
・hidden=8（消えロール状態）で窒息したら、ブロックが消えていく演出を遅らせるようにした。
・Ti式T-Spinではb2bcheck=1でもBack to Backが行えないようにした。
・アイテム「RANDOM」でスカが発動した場合、効果ブロック数を4分の1にした。
・F-pointのツモを某フラッシュの電源パターンに変更しました。
・missionではT-spin判定を強制的に3コーナーにするように
・noviceおよびDEVIL-でロール中に窒息してもスタッフロールを止めないようにした。（そのままクリアまたは足切りになります）
・対戦で勝利条件がLINE数の場合、時間切れ時はLINE数で勝敗を判定するようにした。
・DEVIL-Lv1000ロール速度をDOOM 800に。窒息してもクリアになるからｶﾞﾝｶﾞﾚ
・流れ星の速度を速くしました。マスタープレイ中などに流れるとちょっと見づらくなるので
・アイテム「BOOST FIRE」の内部処理を変更。速度のバックアップを取らなくても良くなった。
・「STAMDARD」を「SIMPLE」に改名
・「ACE-OLD」を「OLD STYLE」に改名

・「ST_rankingGet」の引数不足によりセクションクリア時に落ちる問題を修正
・DEVIL等でレベル1000を超えても結果表示およびリプレイ選択ではLv999と表示されていた問題を修正
・ミッション「BIG」の次で音楽が変わる場合に落ちる問題を修正
・OLD-STYLEでLv15以降に最高速にならなかった問題を修正

C7V1EX YGS2K
バグ修正
・BGMのフェードアウトとか色々ずれてた
・DEVIL800ですぐにせり上がってしまう
・grade４のmをMASTERに
・miniselectで変わらなフォントが普通のときノルマが上がらない
・miniセレクトでせり上がりの切り替えが出来ない
・ベストレコード文字かぶり
・リセットしても[]のまま
・スクウェアモードがマラソンに入る
・ゲームオーバー後に表示されるランキング画面が違う
・practiceにおいてノルマ値後のエンディングが足切り用になってる
・エンドレスなのにノルマ値が設定できる
・いらない項目の時はNODATA
・誤字
リセットできない
・originalで点数が9999999になる
・FPでbasicな状態で他のモードも選べてしまう
・originalの速度ロードバグ
その他
・40LINESとかのクリア後の花火の表示を高速化した
・GMとGmを分けた
・BGMの切り替えをモード別とかGRADE別とかで細かく設定できるようにした
（そのかわりwave2とかwave5とかの部分は簡単化されてるので注意
そういった切り替えはBGM切り替えの部分で出来ます）
・アイテム出現率をiniで設定できるようにした
・devil-にも個別で足切りタイムをつけた
・それに伴ってdevilでの足切りを厳しくした
・devilの1000足切りタイムを500の2倍にした
・認定段位をSTRANKINGファイルに埋め込むのをやめてplayerdata.savを新しく作ってそれに埋め込み

C7V0EXベータ YGS2K
・モードを横方向キーで選択できるようにした。
・各モードのエンドレスをまとめて下に移動。
・T-spinをシンプルにした。
・新ランキングの時色々なモードでランクインされなかったのを多分修正。
・2P側の文字被りとかを色々修正。
・SMALLフォント時を不要なぐらい改造。
・画像ファイルは低解像度未対応(実は作ってないだけ)
BIGINNERモード
・ロール中の花火コンボ発生中に窒息すると花火が止まらなくなるバグをたぶん修正
MASTERモード
・段位を画像を使って表示。
・GMにならないバグ修正　なんどもごめんなさい
・認定試験の時の範囲を間違って指定した時のエラー対策
・段位認定試験を使った認定段位のようなものを付けた。
（毎度のことですがお遊びでつけた段位なので信用してはいけません）
DEVILモード
・death+の段位をdevil-の段位にした。
PRACTICEモード
・PRACTICEにゴール設定を四つ追加
・NEXT生成方法をノーマルとPRACTICEと分けた。
ACEモード
・ACEモードで未クリアの場合にはLINE数を表示。
・ACE-SEGAをオールドスタイル仕様にした。
事前に選んだ回転がワールド系ならGBに、クラシック系ならARCADEになります。
・いつの間にACEのBEST TIME表示が無効になってるバグ修正
VERSUSモード
・余計にアイテム追加。
・対戦を勝利条件にレベルのほかに列消しでも付けられるようにした。
・アイテム出現率をやっぱり重み式にした。
STANDARDモード
・STDモードにどっかが作ったモードを2つ追加。
・STDモードにベストタイムとか色々表示。
・STDの速度ロードのバグ修正。
C7U8EX YGS2K
・アイテム周り
二種類のアイテムセット
一回出たアイテムはしばらく出ないように
凶悪なアイテムは出現率を低くしたつもり
・SQUARE判定を列消しが発生する時も行うようにした。
・GOLDSQUAREの時ノルマが上がらないバグを修正。
・ACE-SEGAモードでbgmが鳴らないバグを修正。
・対戦画面でギブアップボタンを押すと、SPEEDとW1～W3、WTの値が全て０になるバグを修正。
・PRACTICEモードの設定プリセットにBGM項目を追加。

これ以前はchangelog2.txtへ移動
*/

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  外部ライブラリのインポート
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
#include "include.h"

#include "ygs2kfunc.h"
#include "prototypes.h"
#include "inireader.h"

#define		STRING_MAX		200
#define		str				char*
#define		loop			while ( loopFlag )
#define		halt			spriteTime()
#define		time			gametime
#define		ctime			cgametime
#define		stime			sgametime
#define		wait1			gamewait1
#define		wait2			gamewait2
#define		wait3			gamewait3
#define		waitt			gamewaitt

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  グローバル変数の定義
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
int		bgmteisiflg = 0;			//bgm teisi
int		count;					// グローバルカウンタ (フレーム単位、65535まで)
int		backno;					//1～12 通常	61 対戦用

int		stat[2], statc[10 * 2];	// 現在のステータスとパラメータ no + pl * 5
int		sc[2], lv[2], li[2];	// スコア、レベル、ライン数
int		time[2], timeOn[2];		// ゲーム開始からの経過時間 (1/60秒単位)、有効?
int		next[2 * 3];			// nextの部分に入っているブロック #1.60i
int		fld[10 * 22 * 2];		// フィールドの状態  x + y * 10 + pl * 210
int		blk[2];					// 今落ちてくるブロック
int		bx[2], by[2], rt[2];	// ブロックの座標
int		bs[2], bk[2];			// ブロックの自然落下とくっつきのタイム
int		mp[2], mpc[2];			// 前回のパッドの入力内容と時間
int		mps[2 * 2];				// スタート時のmpとmpc（リプレイ記録用）
int		erase[22 * 2];			// 消されるブロック
int		obj[100], objx[100], objy[100], objv[100], objw[100], objj[100], objc[100], obja[100];
int		lc[2], sp[2];			// レベルアップまでのカウンタと落下スピード
int		upLines[2];				// 上昇してしまうライン数
int		wait1[2], wait2[2];		// 固定→次・消滅時の待ち時間
int		wait3[2], waitt[2];		// 設置から固定までのタイム・横溜め時間
int		nextb[1400 * 2];		// 次のブロック(対戦用) playerごと#1.60c7m1
int		nextc[2];				// 次に降るブロックカウンタ
int		hole[2];					// 対戦のときの穴の位置
int		nextwork[7 * 2];		// 次に降ってくるブロックのダブリ防止用
// #1.60cグローバル変数
int		hold[2];				// hold中のミノ
int		dhold[2];				// hold可不可
int		dhold2[2];
int		sw, sh;					// snapshot用 スクリーン幅・高さ
int		ssc = 0;				// ssカウンタ
int		tc[2];					// tgmシリーズ風カウンタ
int		tr[2];					// tgmRank
int		start[2];				// ゲーム開始レベル
int		kickc[2];				// 床蹴り等のカウンタ
//int		kickm = 8;			// World時の再接地可能な回数・接地状態で回転できる回数
int		kickc2[2];				// ARS用床蹴りカウンタ（接地状態の回転回数は数えない）#1.60c7q2ex

// #1.60c6追加変数
int		ending[2] = {0, 0};
int		onRecord[2] = {0, 0};
int		edrec[2] = {0, 0};

// 連続スナップ
int		capc = 0;		// カウンタ
int		oncap = 0;		// 取得中フラグ
int		capKey = 0x46;	// 開始/終了キー
int		capi = 2;		// 取得間隔(フレーム単位)
int		capx = 0;		// 取得領域の左上X座標
int		capy = 0;		// 左上Y座標
int		capw = 320;		// 幅
int		caph = 160;		// 高さ

// #1.60c6.1b追加変数
int		shirase[2];

// #1.60c6.2c追加変数
int		p_shirase[2];	// 1だとPRACTICEモードでも20レベルごとにSHIRASEのせり上がりをする
// #1.60c6.2d追加変数
int		p_bgmlv = 0;	// PRACTICEで流すBGM番号

// TAスコア変数
int		sdrop[2];
int		qdrop[2];
// int		qput[2];	// 未使用？
int		cmbpts[2];

// 開始レベル選択肢
// lvup == 1の場合、全モードレベル100毎(0～1000)
int		startlvTbl[56] = {
	0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130,	// Beginner
	0,  6, 12, 18, 24, 30, 36, 42, 48, 54,  60,  66,  72,  78,	// Master
	0,  6, 12, 18, 24, 30, 36, 42, 48, 54,  60,  66,  72,  78,	// 20G
	0,  3,  6,  9, 12, 15, 18, 21, 24, 27,  30,  33,  36,  39	// Devil
};
int		slv[2],tlv[2];
// ここまで
// BGMフェードアウト開始レベル *K*
int		bgmlv;					// 現在のBGM段階(0:playwave(50)
int		fadelv[2];					// フェードスイッチ兼フェードレベル

// いくつかiniに移転 #1.60c7i5 biginner～devil
int		bgmfadestart[50] = {
	 185, 10000, 10000, 10000, 10000, 10000,	// BEGINNERHANABI
	 285, 10000, 10000, 10000, 10000, 10000,	// BEGINNERSCORE
	 485, 10000, 10000, 10000, 10000, 10000,	// MASTER,20G G1
	 485,   685,   885, 10000, 10000, 10000,	// MASTER,20G G23
	 485,   685, 10000, 10000, 10000, 10000,	// MASTER G4
	  -1,    -1,   485,   685,   985, 10000,	// DEVIL
	  -1,    285,  485, 10000, 10000, 10000		// DEVIL-
};
// いくつかiniに移転 #1.60c7i5
int		Replaybgmfadestart[48] = {
	  99,   129,   149, 10000, 10000, 10000,
	  29,    47,    59, 10000, 10000, 10000,
	  -1,    47,    59, 10000, 10000, 10000,
	  -1,     8,    14, 10000, 10000, 10000,

	 185, 10000, 10000, 10000, 10000, 10000,	// BEGINNER
	 485,   685, 10000, 10000, 10000, 10000,	// MASTER
	  -1,   485, 10000, 10000, 10000, 10000,	// 20G
	  -1,    -1,   485,   685,   985, 10000		// DEVIL
};
//oriフェードアウト
int		ori_bgmfadestart[16] = {
	  99,   129,   149, 10000,
	  29,    47,    59, 10000,
	  -1,    47,    59, 10000,
	  -1,     8,    14, 10000
};

int		color_tbl_max = 40;
int		color_tbl[40] = {
	10, 10, 9, 9, 8, 8, 8, 7, 7, 7,
	6, 6, 6, 5, 5, 5, 4, 4, 4, 4,
	3, 3, 3, 3, 2, 2, 2, 2, 1, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0
};
// ここまで

// hogeパッチ変数 ここから
int		tame1 = 25,	// hoge 左右の入れっぱなしで高速移動するまでの時間
		tame2 = 3,	// hoge 高速移動の速さ(少ないほど速い)。
		tame3 = 25,	// hoge ↑の上下版
		tame4 = 3;	// hoge ↑の上下版
int		mp2[2], mpc2[2];	// hoge 前回のパッドの入力内容と時間(上下キーバージョン)
		// ↓の変数は、旧vsモード時のwait設定に使います。(heboris.iniで設定される)
char	waitname[25][128] = {"DEFAULT", "DEVIL+", "INFINITY", "..!!?", "DEVIL", "!?", "20G"};	// 設定名
int		syutugen[25] = { 3,  5, -1,  3,  5,  6, 26},	// ブロックが固定されてから、次のブロックが出現するまでの時間
		syoukyo[25]  = { 0,  0, -1,  5,  5,  5, 40},	// ブロックが一列揃ったときの消滅の待ち時間
		settyaku[25] = {13, 13, 99, 14, 13, 14, 28},	// ブロックが地面に到達してから、接着されるまでの時間。99にすると無限
		yokotame[25] = { 7,  7, -1,  7,  7,  7, 12},	// 横溜めに必要な時間
		hiddenlv[25] = {-1, -1, -1, -1, -1, -1, -1},	// HIDDEN LV(8まで)
		fps[25]      = {60, 60, 60, 60, 60, 60, 60},	// FPS
		waitkey[25]  = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08},	// ショートカットキー
		blind[25]    = {-1, -1, -1, -1, -1, -1, -1},
		p_bgmload[25]    = {-1, -1, -1, -1, -1, -1, -1};//BGM
// ここまで
// #1.60c6.2c追加変数
int		speed[25] = {1200, 1200, -1, 1200, 1200, 1200, 1200};	// ブロックの落下スピード

// #1.60c6.2f追加変数
int		p_shirase_line = 2;			// SHIRASEで何ラインせり上がるか
int		p_shirase_interval = 20;	// SHIRASEで何レベル毎にせり上がるか
// #1.60c6.2g追加変数
int		p_shirase_start_level = 500;	// SHIRASEでのせり上がり開始レベル
int		uplinetype = 0;		// せり上がりタイプ(0:SHIRASE(デフォルト), 1:TGM+)
int		upLineNo[2];		// せり上がりラインNo
int		upLineNoMax = 32;	// せり上がりフィールドのライン総数
int		upFld[256];			// せり上がりフィールド配列(上限は今のところ128ライン)
// #1.60c7 追加廃止変数
// int		padMode;
// 選択パッド変数廃止。選択パッドは外部ライブラリから変更されうると
// 考えたコーディングが望ましい。

// #1.60c7c追加変数
int		p_backno = 9;		// PRACTICEの背景
int		debug = 0;			// デバッグフラグ

int		p_over1000 = 0;		// PRACTICEでの[ ]ブロック

// #1.60c7e追加変数
int		hide_fps = 0;			// 1でFPSを出さない
int		hide_wait = 0;			// 1でWAITを出さない

// #1.60c7f3追加変数
int		hide_score = 1;			// 1でスコアとライン数表示を出さない
int		show_result = 1;		// 1で結果表示を出す

int		fldsizew[2], fldsizeh[2];	// フィールドサイズ横縦 デフォルト 10 * 20

int		IsBig[2];					// Bigモードフラグ

int		startLvback[2] = {0, 0};	// 選択開始レベルの退避用。一時記憶
//int		IsBigback[2] = {0, 0};	// 同上

//int		startnextc[2];		// 開始時のネクストカウントを保存, リプレイにもセーブする。

int		flag;				// 1にするとタイトルに戻る
int		overcount;
int		combo[2], hebocombo[2], pinch[2];
int		winr, winc, winu, wink;
int		vslevel[2], hidden[2], hiddentm[2];
int		hiddeny[2], hiddenti[2], hiddenc[2];

int		rksc[40], rkbl[40], rktime[40], rklv[40], rkfl[40];	// ランキング用

int		noredraw, fast;
int		csr, cnt, enable[80], category, rankingmode;

int		replayData[60 * 60 * 20 * 2];	// リプレイ保存用データ (最大20分まで)
int		saveBuf[50000];			// セーブ用バッファ
int		playback = 0;			// リプレイ中か?
int		demo = 0;				// デモモードか?
int		demotime = -1;			// デモ表示回数(-1設定なのは最初のデモでビギナーモードを表示するため)
int		setupBak[27];			// セッティング内容のバックアップ用

int		pause[2], hnext[2];		// ポーズしてるかーい!?　hnextはnextで今見えてる数

int		gameMode[2];			// 現在のゲームモード 0:BEGINNER 1:MASTER 2:20G 3:DEVIL 4:VERSUS 5:PRACTICE 6:TOMOYO 7:ACE 8:MISSION

int		limit[8] = {0, 20, 30, 30, 40, 40, 50, 1400};	// ツモ生成用

// #1.60c7f4追加変数
int		level_shirase_start;	// せりあがりの開始レベル
int		raise_shirase_lines;	// せり上がり段数
int		raise_shirase_interval;	// せり上がり間隔

// #1.60c7f5追加変数
int		next_adjust = 1;		// 0にすると初手に黄色、紫、緑が来る
int		p_next_adjust = 1;

// #1.60c7f6追加変数
int		bravo = 0;				// 1だと全消しエフェクトあり
int		showctrl = 0;			// 1ならリプレイ以外でも操作状況表示

// #1.60c7f7追加変数
int		rotate_snd = 0;			// 1で回転音あり

// #1.60c7f9追加変数
int		down_flag[2];			// 0:前回↓を使ってないor↓を離した 1:前回↓を使った

// #1.60c7g3追加変数
char	nextb_list[4096];		// ツモリスト(heboris.ini)
char	nextdengen_list[4096];	// 電源パターンリスト(heboris.ini)
char	nextfp_list[4096];		//FP電源パターン

// #1.60c7g5追加変数
// GM条件：(999になった時) スコア126000＆13分30秒(48600)以内＆gmflag1成立済＆gmflag2成立済

// 昇段条件スコア
int		gscore[18] = {         1400,  4200,   8400,  14000,  21000,  29400,  39200,  50400,
					   63000, 70000, 82400, 100000, 117400, 137000, 158000, 180400, 204200,
					  220000};
// 段位の表示名(geade1,3で使う)
str		gname[20]  = {"9", "8", "7", "6", "5", "4", "3", "2", "1",
					  "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9",
					  "M","GM"};
// デビルモードでの段位の表示名
str		dgname[17]  = {"1", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "S10", "S11", "S12", "S13","M","GM","GOD"};

// 段位名 C7T5(geade2,4で使う)
str		gname2[35] = //バグ対策に二つ多く
{
	 "9",  "8",  "7",  "6",  "5",  "4",  "3",  "2",  "1",	//  0～ 8
	"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9",	//  9～17
	"M1", "M2", "M3", "M4", "M5", "M6", "M7", "M8", "M9",	// 18～26
	 "M", "MK", "MV", "MO", "MM", "GM","GM","GM"		// 27～32
};
// 内部的に使う変数
int		grade[2];			// 現在の段位(0:9 27:M 32:GM)
int		gmflag1[2];			// レベル300突入時に「段位1」 以上 「4分15秒(15300)」以内で1になる
int		gmflag2[2];			// レベル500突入時に「段位S4」以上 「7分30秒(27000)」以内で1になる
int		gmflag1_e[2];		// gmflag1の判定をしたかどうかのフラグ
int		gmflag2_e[2];		// gmflag2の判定をしたかどうかのフラグ
int		gmflag3[2];			//
int		gmflag3_e[2];			//

// #1.60c7g7追加変数
int		timelimit_devil;	// 足きりタイム(devil)
int		timelimit_master;	// 足きりタイム(master)
int		timelimit_20G;		// 足きりタイム(20G)
int		timelimit[2] = {0,0};			// 現在の足きりタイム
int		mpc3[2];			// 前回のパッドの入力時間
int		mpc4[2];			// 前回のパッドの入力時間

// #1.60c7h2追加変数
int		repversw;			// 旧バージョンのリプレイデータを再生するためのスイッチ

// #1.60c7h3追加変数
int		deadtype = 0;		// 死亡アニメ (0=下からブロック消去 1=灰色化)

int		limit15 = 1;		// 15分以上プレイすると超高速 (0=無効 1=有効)
							// (リプレイに保存されます)

// #1.60c7h8追加変数
//int		flag_over1000[3 * 2];	// NEXTの出現LV
int		breaktype = 0;			// 飛び散るエフェクトの種類 0:TGM 1:TAP/TI
// int		wblock;
int		shadow_heboris = 0;		// シャドウロール

// #1.60c7i1追加変数
int		enable_hold_frame = 1;	// HOLDした時に黄色の枠を0=表示しない 1=表示する
int		enable_grade[2];		// 段位認定 0=なし 1=あり
int		hold_used[2];			// HOLDを使ったかフラグ 0:まだ使ってない 1:初回 2:２回目以降

// #1.60c7i2追加変数
int		bdowncnt[2];			// ミノを着床させた回数
int		st_bdowncnt[2];			// STランキング用（bdowncntと違って定期的にリセットがかかる）
int		tlsrange = 0;			// PRACTICEでのTLSの範囲 0:OFF 1:100まで 2:ALWAYS
int		gmflag_r[2];			// ロールクリアでGMになる権利のようなもの
int		semigmflag_r[2];		// 見えるロールクリア用

// #1.60c7i6追加変数
int		giveup_func = 0;		// 捨てゲーした時の動作(0=参加待ち 1=モードセレクト 2=タイトル)
int		giveup_safety = 0;		// 0=どんな状況でも捨てゲー可能 1=ポーズ中だけ捨てゲー可能.
int		mini_select = 0;		// SOLO MODEをTIME TRIAL専用にする設定(0=無効 1=有効)

// #1.60c7i8追加変数
int		IsBigStart[2] = {0, 0};	// BIGで始めたかどうかのフラグ(DEVILのバグ回避用)

// #1.60c7i9追加変数
int		skip_viewscore = 0;		// スコア・タイム表示などの処理を実行しない
int		skip_viewbg = 0;		// 背景を表示しない
int		skip_obj = 0;			// 全ての演出をしない
int		hide_hidden = 0;		// HIDDEN LV表示を隠す

// #1.60c7j1追加変数
//int		time_gmflag1 = 15300;		// レベル300突入時のGM条件タイム
//int		time_gmflag2 = 27000;		// レベル500突入時のGM条件タイム
//int		time_grandmaster = 48600;	// レベル999達成時のGM条件タイム(810*60)
//int		grade_gmflag1 = 8;			// レベル300突入時のGM条件段位
//int		grade_gmflag2 = 12;			// レベル500突入時のGM条件段位

// #1.60g7j2追加変数
int		mirror = 0;					// 鏡像を表示する設定(0=無効 1=有効)
int		domirror = 0;				// mirrorが有効の時、鏡像を表示するフラグ

// #1.60c7j5追加変数
int		p_hold = -1;				// 初期HOLDブロック(-1=なし 0=赤, 1=橙, 2=黄, 3=緑, 4=水, 5=青, 6=紫)
int		bakhold = -1;				// 設定されているHOLDブロック
int		p_nextpass = 0;				// ツモ送り有無
int		pass_flg[2] = {0, 0};		// ツモ送りフラグ(連続入力防止用)
int		fmirror = 0;				// フィールドミラーを使うか
int		p_fmirror_interval = 3;		// フィールドミラー間隔
int		p_fmirror_timer = 20;		// フィールドミラータイマー(ミラー化にかかる時間)
int		rollroll = 0;				// ロールロールを使うか
int		p_rollroll_interval = 4;	// ロールロール間隔
int		p_rollroll_timer = 30;		// ロールロールタイマー(回転するまでの時間)
int		p_xray_interval = 2;		// X-RAY間隔
int		p_xray_frame = 36;			// X-RAY周期フレーム数
int		isrollroll[2] = {0, 0};		// 現在のツモがロールロールか
int		xray = 0;					// X-RAYを使うか
int		isxray[2] = {0, 0};			// 現在X-RAY状態か
int		xray_counter[2] = {0, 0};	// X-RAY用カウンタ
int		color_counter[2] = {0, 0};	// COLOR用カウンタ
int		fcolor = 0;					// COLORを使うか
int		iscolor[2] = {0, 0};		// 現在COLOR状態か
int		shadow_timer = 0;			// シャドウタイマーを使うか
int		p_shadow_timer = 300;		// シャドウタイマー(ブロックが消えるまでの時間)
int		fldt[10 * 22 * 2];			// フィールドタイマー
int		fldbuf[10 * 22 * 2];		// フィールドバッファ(ミラーの演出に使用)
int		fldtbuf[10 * 22 * 2];		// フィールドタイマーバッファ(ミラーの演出に使用)
int		ready_start = 0;			// ready開始
int		ready_end = 15;				// ready終了
int		go_start = 45;				// go開始
int		go_end = 60;				// go終了

// 961追加変数
int		skill[2];		// 4段消し回数(15回毎にCu, Ag, Au, Rg(?) medal)
int		start_flag[2];	// 開始時レベルが上がらないようにする為のFlag

// #1.60c7j7追加変数
int		lap_time[100 * 2];			// HeboLv 3-78, TGMLv 50-1300
int		split_time[100 * 2];		// HeboLv 3-78, TGMLv 50-1300
int		st_record_interval_tgm = 5;	// 50TGMLv毎にタイム更新→これの10倍毎にタイム更新
int		st_record_interval_heb = 3;	//  3HebLv毎にタイム更新
int		hide_st_record = 1;			// 1で記録タイムを見せない

// #1.60c7j8追加変数
int		big_block_type = 1;				// BIGの操作中ブロック描画方法(0=通常ブロック×4 1=2倍拡大)
int		st_record_force_viewright = 0;	// 1で記録タイムをシングル台でも右側に出す

// #1.60c7j9追加変数
int		shadow_timer_type = 0;			// シャドウタイマーの種類 (0=パッと消える 1=徐々に消える)
int		shadow_timer_flag[2];			// シャドウタイマー中かどうか

// #1.60c7k4追加変数
int		allclear[2];					// 全消しした回数

// #1.60c7k7追加変数
int		p_ending = 0;					// PRACTICEでのエンディング
						// (0=通常プレイ 1=BEGINNERロール 2=見えロール 3=消えロール 4=DEVILロール)
int		p_goaltype=0;			//終了方法;なし、レベル、ライン、ブロック、タイム
int		p_goaltypenum;		//上で決めた終了方法がどのの値になったら終わりか
int		p_goaltypenumlist[10]={5,10,20,30,50,75,100,130,160,200};

// #1.60c7k9追加変数
//int		limit15_tt = 1;					// タイムトライアルでの遅延防止の有無(0=なし 1=あり)

int		dead_blockframe = 1;			// 死んだとき枠を消すか(0=消さない 1=消す)

// #1.60c7l1追加変数
int		objp[100];						// 破壊エフェクト用プレイヤー番号

// #1.60c7l2追加変数 [バックアップ]
int		b_wait1[2]={26,26};		// 固定→次
int		b_wait2[2]={40,40};		//消滅時の待ち時間
int		b_wait3[2]={28,28};		//設置から固定までのタイム
int		b_waitt[2]={15,15};		//横溜め時間
int		b_sp[2]	= {1,1};
int		b_nextc[2];		// [バックアップ] 落下スピード,NEXT

// #1.60c7l5追加変数
int		stage[2];	// ステージ番号
int		sclear[2];	// ステージクリアしたか

int		stime[2];	// ステージタイム
int		ltime[2];	// リミットタイム
int		ctime[2];	// クリアタイム

// #1.60c7l7追加変数
int		isfmirror[2];		// 現在の面がミラーか
int		rollroll_flag[2];	// 現在の面がロールロールか
int		xray_flag[2];		// 現在の面がX-RAYか#1.60c7p9ex2
int		color_flag[2];		// 現在の面がCOLORか#1.60c7p9ex2
int		rest_pblock[2];		// 残りプラチナブロック数

int		s_ready_start = 15;			// tomoyo ready開始
int		s_ready_end = 30;			// tomoyo ready終了
int		s_go_start = 135;			// tomoyo go開始
int		s_go_end = 150;				// tomoyo go終了

int		dummy;	// 設定ファイルが空だと23行目で発生する謎のエラーを回避するためのダミー変数

// #1.60c7l8追加変数
int		start_stage[2] = {0,0};		// スタート時のステージ番号
int		skipKey = 0x3F;				// ステージスキップキー

// #1.60c7l9追加変数
int		start_nextc[2] = {0,0};		// スタート時のNEXTC
int		stage_nextc[2] = {0,0};		// ステージスタート時のNEXTC
int		randommode[2] = {0,0};		// ランダムモード

// #1.607m3追加変数
int		clearnum[2];	//スキップせずにクリアした数
int		clearp[2];		//クリア率

// #1.60c7m5追加変数
int		t_noborder[2] = {0,0};		// TOMOYOモードの足切りなしモード 0=無効 1=有効

// #1.60c7m6追加変数
int		BigMove[2];					// BIG時の移動単位 0=2マス単位移動 1=1マス単位移動
int		bigtype = 1;				// ↑のini設定用

// #1.60c7m7追加変数
int		comboM[2];

// #1.60c7m8追加変数
int		combo2[2];					// 1列消しを含まないコンボ

// #1.60c7m9追加変数
int		medaltype = 0;				// メダルの絵の種類
int		world_drop_snd = 0;			// ワールドルールでの接地音の有無(0=なし 1=あり 2=即落下以外あり)
int		super_breakeffect = 0;		// TOMOYO以外でも全ブロックに破壊エフェクトを表示する

int		c_cblk[2];					// 操作中テトラミノの色（操作中）
int		c_cblk_r[2];				// 操作中テトラミノの色（置いた後）
int		c_nblk[6 * 2];				// NEXTミノの色
int		c_hblk[2];					// HOLDミノの色

// #1.60c7n1追加変数
int		vs_match = 0;				// 対戦回数
int		vs_win[2] = {0,0};			// 勝った回数
int		fldi[10 * 22 * 2];			// フィールドアイテム
									// 1つでもアイテムを消すと、他のアイテムは全て消えます。
									// 0=なし 1=MIRROR 2=ROLL ROLL 3=DEATH BLOCK
int		item[2];					// 現在のアイテム
int		item_name[2];				// フィールド右上に表示するアイテム名
int		item_g[2];					// アイテムゲージ
int		fldibuf[10 * 22 * 2];		// フィールドアイテムバッファ(ミラーの演出に使用)
int		item_t[2];					// アイテムの効果時間
int		item_interval = 20;			// アイテムが出現する間隔
int		item_rollroll[2];			// アイテムでロールロール発生中か
int		item_monochrome[2];			// アイテムで[]発生中か

// #1.60c7n2追加変数
int		monochrome_mode[2] = {0,0};		// モノクロモード0
int		p_monochrome_mode;		// 0:普通 1:モノクロ 2:[]
int		hide_tomoyo_totaltime = 0;	// TOMOYOでTOTAL TIMEを隠す
// #1.60c7n3追加変数
int		laststage[2];		//ラストステージ初期値
int		rescue[2];			//REカウンター
int		REflag[2];			//ピンチになると1カウントすると0に

int		pinch2[2];


// #1.60c7n6追加変数
int		tspin_c[2];			// T-SPIN!残り表示フレーム数
int		tspin_flag[2];		// T-SPINの権利みたいなもの
							// Ｔ字ブロックが接地した状態で回転すると1になり、移動すると0になる
							// 1の状態でライン消しすると「T-SPIN!」と表示

// #1.60c7n7追加変数
int		disable_hold = 0;	// HOLD禁止設定
int		disable_irs  = 0;	// 先行回転禁止設定
int		max_fps = 60;		// PRACTICEモードのFPS

// #1.60c7n8追加変数
int		replay_save[2];		// リプレイにセーブ可能かどうか
int		time2[2];			// TOMOYOリプレイ用プレイタイム

// #1.60c7o1追加変数
int		sgrade[2];			// 裏段位 窒息時に判定
int		min_sgrade = 4;		// この段位から裏段位を表示

// #1.60c7o2追加変数
int		cp_x[2];				// 最善手のX座標
int		cp_rt[2];				// 最善手のブロックの向き
int		cp_input[10 * 2];		// コンピュータの入力状況
int		cpu_flag[2];			// 操作しているのがコンピュータかどうか

// #1.60c7o3追加変数
int		cp_hold[2];					// コンピュータがHOLDするときは1
int		cp_player_1p = 0;			// コンピュータが操作するか(1P)
int		cp_player_2p = 1;			// コンピュータが操作するか(2P)

// #1.60c7o4追加変数
int		cp_type = 0;				// コンピュータの性格 0=とにかくラインを消す 1=2ライン以上消えるときだけ
int		cp_rot_c[2];

// #1.60c7o5追加変数
int		tmp_maxPlay;	// プレイ人数のバックアップ シングル台で対戦するときに使用
int		block_rframe = 0;	// ブロックの周りに白い四角形を表示

// 以下hogeパッチより
int		sevolume = -1500;	//効果音のボリューム。0が標準、-10000で無音。
int		se = 1;			//効果音を流すかどうか。(0なら流さない)
int		bgm = 1;		//BGMを流すかどうか。(0なら流さない)

// #1.60c7o6追加変数
int		endtime[2];		// スタッフロール経過時間

// #1.60c7o7追加変数
int		t_training[2];	// TOMOYOモードを1面限定で遊ぶかどうか

// #1.60c7p1追加変数
int		up_flag[2];			// 0:前回↑を使ってないor↑を離した 1:前回↑を使った
int		p_stage = -1;		// PRACTICEで使うステージ(0:なし 1-27:TOMOYOステージ 28-99:PRACTICEステージ)

// #1.60c7p2追加変数
int		bgfadesw = 0;		// 背景フェードスイッチ 0:通常 1:フェードアウト 2:フェードイン
int		bgfadelv = 0;		// 背景フェードレベル 0-256まで
int		enable_bgfade = 0;	// 背景フェードを 0=使わない 1=使う
int		bgfade_cnt = 0;	// 背景フェードの速さ 数字が大きいほど高速

// #1.60c7p3追加変数
//int		bgfade_type = 38;	// 背景フェードの種類

// #1.60c7p4追加変数
int		english = 0;		// For English Users

// #1.60c7p8追加変数
int		bgmvolume = -1500;		// BGMのボリューム 0が標準、-10000で無音
int		bg_max = 9;				// 背景の最大数

// #1.60c7p9ex追加変数
int		xray_bgblack = 1;		// X-RAY時に背景を真っ黒にする
int		xray_drawblockframe = 1;// X-RAY時にブロックの枠線を描く
int		color_bgblack = 0;		// COLOR時に背景を真っ黒にする

// #1.60c7q1ex追加変数
int		stp[2];		//STプラチナメダル個数
int		stg[2];		//ST金メダル個数
int		sts[2];		//ST銀メダル個数
int		stb[2];		//ST銅メダル個数

// #1.60c7q2ex追加変数
int		disable_wallkick = 0;	// 1にすると壁蹴り無効

// #1.60c7q5追加変数
int		item_nblk[6 * 2];
int		item_coming[2];
int		item_waiting[2];

int		hanabi_waiting[2];	// 発生待ち花火数
int		hanabi_frame[2];	// 発生するまでのフレーム数

int		noitem = 0;			// 1にするとアイテムが無しになります＠VERSUS MODE
int		no_hanabi = 0;		// 1にするとラインを消しても花火が表示されなくなります＠BEGINNER MODE

// #1.60c7q6追加変数
int		r_irs = 1;			// 1にすると赤のIRSをTGM基準化（床蹴りなし）

// #1.60c7q7追加変数
int		st_new[20 * 2];		// 左上のセクションタイム表示の色を変えるときに使う
int		max_fps_2 = 60;		// 最大FPS

// #1.60c7q8追加変数
int		timelimit2_devil = 0;	// LV1000足きりタイム(devil)
int		timelimit2[2] = {0,0};			// 現在のLV1000足きりタイム

// #1.60c7r2追加変数
int		big_rise = 1;			// BIG時にせり上がり2倍

// #1.60c7r2+追加変数
int		anothermode[2] = {0,0};		// アナザーモード
int		timelimitw_devil = 0;	// ワールド足きりタイム(devil)
int		timelimitw[2] = {0,0};			// 現在のワールド足きりタイム

// #1.60c7r4追加変数
int		vsmodesl = 0;		//vsモードセレクト
int		disrise = 0;		//競り上がりなし
int		sptmp[2] = {0, 0};		//VSモード速度テンプレート

// #1.60c7r5追加変数
int		vs_goal = 200;		// VSモード ゴールレベル（0=なし）
int		vs_time = 5400;		// VSモード 制限時間（0=無制限）
int		breakti = 0;		// breaktype = 1のときの破壊エフェクトの種類
int		wintype = 0;

// #1.60c7r7追加変数
int		world_i_rot = 0;	// ワールドルールのI型ミノの回転補正タイプ 0=左右対称タイプ 1=非左右対称タイプ

// #1.60c7r8追加変数
int		c_mission = 0;		// 現在のミッション番号
int		c_norm[2];			// 現在達成しているノルマ
int		mission_file = 0;	// ファイル番号

int		mission_type[40];	// 問題の種類
int		mission_norm[40];	// 問題のノルマ
int		mission_time[40];	// 問題の制限時間
int		mission_end[40];	// 0=クリアすると次の問題へ 1=クリアするとエンディング 2=最後をダブル消し以外でエンディング
int		mission_lv[40];		// 落下速度レベル
int		mission_bgm[40];	// BGM

// #1.60c7r9追加変数
int		cycle[4];			// サイクル用
int		mission_erase[40];	// 上から消去するライン数

// #1.60c7s2追加変数
int		mission_opt_1[40];	// ミッションによって異なる追加情報
int		mission_opt_2[40];
int		mission_opt_3[40];
//int		mission_opt_4[30];

int		target_cleared[200];	// ターゲットのステージ出現済みフラグ（同じ面がまた出るのを防ぐ）

int		eraser_lines[4];	// イレイサーのライン位置
int		eraser_cleared[4];	// 既に消したかどうかのフラグ
int		eraser_now_pattern = 0;	// イレイサーの現在のパターン

int		spawn_y_type = 1;	// 出現位置タイプ（死亡判定も変わります）

// #1.60c7s3追加変数
int		tls_view = 0;		// TLS 0=自動 1=常に有効 2=常に無効

// #1.60c7s4追加変数
int		clear_mission;		// クリアしたミッション数
int		eraser_now_lines;	// 現在画面内に存在しているイレイサーのラインの数

int		ybigtype = 1;		// BIG時の縦移動単位 0=2マス単位移動 1=1マス単位移動

// #1.60c7s5追加変数
int		over1000_block[2];
int		over1000_start;		// []の開始レベル

// #1.60c7s6追加変数
int		upLineT[2];				// せり上がりタイプ
int		tspin_type;			// T-SPINの判定方法

// #1.60c7s6追加変数
int		hanabi_combo[2];		//HANABIコンボ補正用記憶域
int		hanabi_flag;			//HANABIを一回の列消去につき一回の発生のみ
int		hanabi_total[2];		//HANABI合計値
int		endingcnt[2];			//エンディング中の花火なりすぎるのを止める

//int		player;
int		tomoyo_hanabi[2];		//TOMOYOモード用HANABIカウンタ

int		last_tspin[2];			//T-Spinによるライン消し数
int		b_to_b_flag[2];			//Back to Backフラグ
int		b_to_b_c[2];
int		b2bcheck;			//backtobackをチェックするか(ini)？

int		itemappear_c[2];			//アイテム出現時のゲージフラッシュ時間

int		platinaerase_cnt[2];	//プラチナブロック消去サウンドを鳴らす回数
int		ext_s[2];				//時間増加の演出（表示）用
int		timeextend_snd_cnt[2];	//時間増加の演出（SE）用

int		fmirror_cnt[2] = {-20,-20};			//フィールドミラー用カウンター

int		staffroll_y_ex[2];		//高解像度用スタッフロールy座標
int		tomoyo_opt[2];			//TOMOYOモード開始ステージなど

int		nmlroll_bgmloop = 0;	//普通ロールのBGMをループさせる
int		bgnroll_bgmloop = 0;	//BEGINNERロールのBGMをループさせる

int		top_frame = 0;			//上部フレーム（0=TI 1=ACE）

int		heboGB[2];				//missionのオールドスタイルで使用

int		isrotatelock[2];		//回転封じ
int		isholdlock[2];			//HOLD封じ
int		istimestop[2];			//時間停止
int		ishidenext[2];			//NEXT隠し
int		ismagnetblock[2];		//マグネット
int		isLRreverse[2];			//左右逆転
int		isUDreverse[2];			//上下逆転
int		isboost[2];				//ブーストファイア
int		isfever[2];				//フィーバー
int		isremote[2];			//リモートコントロール
int		isdark[2];				//ダーク
int		istrance[2];			//トランスフォーム
int		isdouble[2];			//ダブル
int		ismiss[2];			//miss
int		isfakenext[2];			//fakenext

int		stopmirror_flag[2];		//対戦用ミラーを終了するフラグ

int		item_timer[2];			//時限性アイテムのタイマー

int		isUPdelfield[2];		//↑DEL FIELD
int		isDWdelfield[2];		//↓DEL FIELD
int		isdeleven[2];			//  DEL EVEN
int		isreflect[2];			//REFLECT
int		isallclear[2];			//ALLCLEAR

// 段位ポイントが1つ減るまでの時間
int		glimit[33] =
{//
	100, 80, 80, 60, 50, 50, 40, 40, 40,		//  0～ 8
	30, 30, 25, 25, 20, 20, 20, 15, 15,		//  9～17
	10, 8, 5, 5, 5, 5, 5, 5, 4,		// 18～26
	5 , 5, 4,3,2,1			// 27
};

int		gpoint[2];			// 現在の段位ポイント
int		gtime[2];			// 段位ポイント減少タイマー
int		gflash[2];			// 段位が上がった時に表示を光らせる時に使用

int		vsbg = 0;	//対戦に専用の背景を使用（0=…せずに、1番目を使用 1=専用の物を使用）

int		use_item[2];	//使用するアイテム 0でランダム

int		p_erase_c[11];	//PRACTICE用 消去ライン数カウント
int		p_heboGBlv = 0;		//PRACTICE用 HeboGB

int		isblind[2];		//ブラインドON/OFF制御
int		blind_flag[2];	//ブラインドの有無

int		p_blind = 0;

int		block_spectrum = 0;	//ブロックの残像（0=OFF 1=ON）
// #1.60c7t7追加変数
int		p_setting;			//practice用のFAVORITESの種類p_setting
int		def_p_sp;			//デフォルトのpractice用のFAVORITESの種類
int		skip_fwait;			//practice用のFAVORITESの表示制限

int		skillg[2];		//gradeup2に使うskill
int		skillbai[2];		//skillgup2で決定される倍率
int		endlines[2];		//ロール中に消した列数
int		gup_down[2];		//cool:1とregret:0

int		start_mission;		//開始時のミッション番号

int		randseed[2];		//乱数シード プレイヤーごと
int		first_seed[2];		//最初のシード

int		title_acnt = 0;
int		title_mov_f = 0;
int		mov_inter = 0;

int		max_hnext[2];
int		death_plus[2];

// 背景ムービー
int		back_acnt = 0;			// 現在のフレーム
int		back_mov_f[13];			// 最大コマ数（0にすると静止画）
int		back_mov_inter[13];		// 1コマを表示するフレーム数

// #1.60c7t9追加変数
int		time99[2];			// レベルストップ時間
int		timeN[2];			// ブロックを操作している時間(0-98)
int		ave_laptime3[2];	//セクションの平均移動時間
int		gup3rank[2];			//ランク0-2
int		gup3sp_adjust[2];		//ランクにあわせてスピード調整
int		qualityflag[2];			//2回以上出ないように
int		skillflag[2];
int		tr2[2];					//可変速レベル
int		border_rank[2];			//
int		border_time[20] =		//ランクごとの基準タイム
{
	17, 15, 14, 13, 10, 10, 12, 13, 13,		//  0～ 8
	13, 13, 13, 13, 13, 13, 12, 12, 11,		//  9～17
	11, 10		// 18～20
};
int		border_time59[20] =		//ランクごとの基準タイム
{
	17, 15, 14, 13, 10, 10, 11, 12, 12,		//  0～ 8
	12, 12, 12, 12, 12, 11, 11, 10, 10,		//  9～17
	9, 9		// 18～20
};
int		border_time48[20] =		//ランクごとの基準タイム
{
	17, 15, 14, 13, 10, 10, 10, 9, 9,		//  0～ 8
	9, 9, 9, 9, 9, 9, 9, 9, 9,		//  9～17
	9, 9		// 18～20
};
int		grade2[2];			//19段変換
int		textguide;			//テキストガイド

int		min_speed[2] = {0,0};
int		isthunder[2];
int		thunder = 0;
int		thunder_timer;

int		enable_minspeed = 1;		//TOMOYO最低速度を有効にする

int		tomoyo_rise_fld[10 * 20 * 2];	//TOMOYO用せり上がりフィールド 最大20段
int		tomoyo_rise[2];					//せり上がりON/OFF兼せり上がり間隔
int		tomoyo_rise_t[2];
int		hnext_flag[2];
int		hnext_timer[2];
int		tomoyo_waits[2];
int		tomoyo_domirror[2];
int		tomoyo_ehfinal_c[2];

int		ndelay[2];
int		navigation = 1;
int		navigation_time[2];
int		navigation_limittime = 60;
int		navigation_limitLv = 100;
int		hold_snd = 2;

int		nanamedown = 1;
int		hide_tomoyoinf = 1;
int		erasecnt[2];

int		item_mode[2];	// アイテムモード
int		hebo_plus[2];	// TGM+

int		fastmirror = 0;	// ミラーのフィールド回転を高速化　合計時間は同じ

int		beginner_rollbgm = 0;	//ビギナーのロールBGM変更 (0=専用 1=bgm02 2=通常ロール)
// #C7U0.5EX追加変数
int		rankingrule;
int		ranking_type;				//ランキング表示タイプ

int		medal_ac[2];				// acメダルの色
int		medal_sk[2];				// skメダルの色
int		medal_co[2];				// coメダルの色
int		medal_re[2];				// reメダルの色
int		medal_st[2];				// stメダルの色(上から全てまとめて)
int		end_f[2];				// 完全クリアフラグ

int		disable_ehlast_gimmick = 0;

int		lasernum[2];			//レーザーの本数
int		laserpos[4 * 2];		//レーザーの位置
int		rapid_c[2];				//ボタン連打数（レーザーの増加に使用）
int		dolaser[2];				//ブロック移動中以外のレーザー発射フラグ
int		do16t[2];				//ブロック移動中以外の16tフラグ

int		nega_pos[10 * 2];		//ネガフィールド関連
int		donega[2];

int		shotgunpos[22 * 2];		//ショットガン関連
int		doshotgun[2];

int		doexchg[2];				//フィールド交換
int		docopyfld[2];				//コピー

int		rt_nblk[6 * 2];			// NEXTブロックの向き（ROTATE LOCKに使用）

int		isbanana[2];			//SHUFFLE FIELD関連
int		banana_pos[10 * 2];
int		banana_c[2];

int		enable_randexam = 1;	//1にすると一定の確率で段位試験発生
int		examination[2];			//試験中？0:なし1:昇格試験　2:降格試験
int		exam_grade[2];			//試験段位
int		purupuru[2];			//フィールドのプルプル
int		exam_range[8] = {		//試験段位の範囲
			9 , 18 ,			//段位1設定時
			9 , 28 ,			//    2
			6 , 15 ,			//    3
			9 , 27 				//    4
		};

int		ace_irs = 0;			//ACE式IRS 1はACE 2はカスタム

int		stage_skip_mpc[2];	//長押し
int		hebop_interval[2];	//HEBO+せり上がり感覚
/* STランキングに使う変数 */
int		stage_time[30 * 2];	//ステージごとのタイム
int		lvstop_time[20 * 2];	//レベルストップタイム
int		st_other[30 * 2];		//落としたブロック数

int		harddrop[2];
int		versus_rot[2];

int		tr_20g[2];
int		dorulet[2];

int		Isbesttime;		//ベストタイムを見せるか

// ACEモードBGMフェードアウトテーブル
int		ace_bgmfadestart[4 * 4] = {
	   45,    95,   145, 10000,		// NORMAL
	   35,    95,   145, 10000,		// ANOTHER
	   45,   145,   195, 10000,		// ANOTHER2
	   45,   396,   10000, 10000,		// OLD
};
// ACEモードBGM切り替え用
int		ace_bgmlist[3 * 4] = {
	0, 1, 2,	// NORMAL曲
	5, 3, 4,	// ANOTHER曲
	19,18,16,	// ANOTHER2
	8,9,9,	// OLD
};
int		ace_bgmchange[2];

// READY→GO演出の種類　（0=HEBO 1=TI）
int		ready_go_style = 1;

int		ti_ready_start = 20;		// 通常 ready開始
int		ti_ready_end = 75;		// 通常 ready終了
int		ti_go_start = 80;		// 通常 go開始
int		ti_go_end = 140;		// 通常 go終了

int		ti_s_ready_start = 45;		// TOMOYO ready開始
int		ti_s_ready_end = 155;		// TOMOYO ready終了
int		ti_s_go_start = 160;		// TOMOYO go開始
int		ti_s_go_end = 225;			// TOMOYO go終了

int		r_start[2];
int		r_end[2];
int		g_start[2];
int		g_end[2];

int		bk_D[2];		// D.R.S用　下入れによる固定時間
int		p_item_mode = 0;
int		are_skipflag[2];

int		def_vs_goal = 200;

int		hanabi_int[2];		//ロール中の花火打ち上げ間隔

int		isFreefall[2];	// フリーフォール
int		Ff_rerise[2];	// ↑の消去後再せり上げライン数
int		isLmovfield[2];	// ←MOV FIELD
int		isRmovfield[2];	// →MOV FIELD
int		do180field[2];	// 180°フィールド

int		ofs_x[2];		// ↑の演出用
int		ofs_y[2];
int		ofs_x2[2];
int		ofs_y2[2];
int		stp_point[2];		//STプラチナ獲得のためのポイント

int		m_roll_blockframe = 1;	//見え＆消えロールでブロックの枠を表示（消えは一瞬）1でON
int		kickc3[2];

int		fldu[10 * 22 * 2];	// せり上がりフィールド（せり上がるブロックの形状を記憶）
int		fldubuf[10 * 22 * 2];
int		upLines_waiting[2];
int		regretcancel[2];
int		def_vs_uplinetype = 3;

int		devil_minus[2];

int		devil_randrise = 1;	// DEVIL(DOOM)のせり上がり間隔をセクションごとのランダムにする 1=ON
int		devil_nextrise[2];

int		bgmload[20] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};	//指定したBGMを読み込むか
int		exam_ranking = 1;	//試験時のランキング登録有無

int		novice_mode[2];		//noviceモード
int		novice_flag[2];		//noviceのアイテムフラグ

int		FP_bonus[2];
int		FP_bonus_c[2];

int		FP_next[4] = {0,2,4,6};

int		disable_giji3D = 0;	//ブロックの立体表示無効設定（0=全て表示　1=BIGのTLSのみ無効　2=TLSのみ無効　3=フィールド上のブロックのみ有効　4=全て無効）
int		alpha_tls = 2;		//TLSを半透明にするか（0=暗い不透明　1=BIGのみ暗い不透明、他は半透明　2=半透明）
int		bo[2];
int		fastroll[2];		//高速スタッフロール用
int		fpbas_mode[2];		//FP基本モードfpbas_mode

int		std_opt[2];		//STDモード
int		flds[10 * 22 * 2];		// スクウェア用フィールドの状態  x + y * 10 + pl * 210
int		fldsbuf[10 * 22 * 2];		// スクウェア用フィールドの状態  x + y * 10 + pl * 210

int		squaremode[2];			//四角用
int		itemhistory[2*5];		//アイテム履歴
int		item_pro[50]={5,8,5,9,6,3,5,4,6,8,
					  4,7,5,6,5,3,6,6,5,4,
					  9,1,5,1,7,7,8,3,3,5,
					  7,4,3,3,1,5,3,2,7,   5,
					     5,5,5,5,5,5,5,5,5,5};		//アイテム出現率（ini出設定）
int		p_nextblock;		// 練習用ツモ決定
int		squarecnt[2];		//スクウェアカウント
int		ori_opt[2];			//オリモードオプション
int		admit_grade[2]={0,0};			//認定段位
int		grade_his[2*5]={0,0,0,0,0,0,0,0,0,0};			//段位履歴
int		grade_pasttime[2]={0,0};		//試験からどれだけ経ったか？
int		scrate[2];				//oriモード用コンボスコアレート
int		timelimitm_devil = 0;	// devilm足きりタイム
int		timelimitm[2] = {0,0};			// 現在のdevilm足きりタイム

int		timelimitmw_devil = 0;	// devilm足きりタイム//ワールド
int		timelimitmw[2] = {0,0};			// 現在のdevilm足きりタイム

int		item_num = 39;		//アイテムの個数	playerinitialで初期化処理を行うようにすること

int medal_info = 0;		//メダルに関する情報を表示 0=しない 1=する
int timelimit_doom_E = 240 * 60;	//devil-DOOM新足切りタイム-Easy(ACE-SRS、ACE-ARS、DS-World)
int timelimit_doom_N = 205 * 60;	//						  -Normal(Heboris、Ti-World、ACE-ARS2)
int timelimit_doom_H = 183 * 60;	//						  -Hard(Ti-ARS、D.R.S、SRS-X)

int dispLinfo = 1;		//リプレイ中のライン消去情報の有無

int relaymode[2];
int first_rot[2];
int relaydata[9 * 2];
int relayround[2];

int vs_style[2];	// 対戦のスタイル　0=NORMAL　1=ATTACK　2=DEFENCE　3=ITEM
int item_inter[2];	// プレイヤー毎のアイテム出現間隔（スタイルで間隔が変わるため）

int vs_round;	//何ラウンド目か
int winpoint;	//勝利となるポイント数
int vs_points[2];	//現在のポイント数
int misstimer[2];	//アイテム「スカ」用のitem_t

int repdata[20 * 2];

int fade_seed = 20;	// BGMフェードアウト用

int se_play[50];

// C++用設定
int cpp_texpow2 = 1;		// [OpenGL] テクスチャの大きさを2の累乗に限定する
int cpp_texdxt = 0;			// [DirectX] テクスチャをDXT圧縮する
int cpp_texfilter = 0;		// [DirectX][OpenGL] 拡大された画像の表示形式

// 文字列定数
str		version = "C7V4EX " FRAMEWORK_VER " 07/11/23";	// 現在のスクリプトのバージョン(ver+date形式、1.60は除く)
str		RankString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,- 0123456789<=>?!#$%&'()=pq";

/* 定数 */
int		item_pronum;		//item_proの合計　面倒だから手動で計算
int		fldisno = 44;		//fldiにおいてスクウェア用の画像がある場所(場所のカウントは→矢印が30番目)
int		fldigsno = 45;		//fldiにおいてGOLDENスクウェア用の画像がある場所
int		fldihardno = 43;	//fldiにおいてハードブロックの画像がある場所

bool	loopFlag = true;			// false になると何もかも無理矢理抜ける
char	*string[STRING_MAX];

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ソースファイルのインポート
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
#include "speed.def"				// レベル調整
#include "script/mission_info.def"// ミッション情報

#include "script/flexdraw.c"		// 複数解像度対応描画関数	#1.60c7p9ex
#include "script/config.c"		// Config	#1.60c5
#include "script/world.c"		// Worldルール #1.60c4
#include "script/classic.c"		// クラシックルール
#include "script/ars.c"			// ARSルール
#include "script/classic_D.c"	// D.R.S
#include "script/effect.c"		// 演出処理
#include "script/sound.c"		// 音の処理
#include "script/practice.c"		// PRACTICEモード
#include "script/tomoyo.c"		// TOMOYOモード
#include "script/ranking.c"		// ランキング
#include "script/readdef.c"		// 設定初期化
#include "script/replay.c"		// リプレイ
#include "script/staffroll.c"	// スタッフロール
#include "script/cpu.c"			// コンピュータ操作
#include "script/sectime.c"		// セクションタイムランキング
#include "script/mission.c"		// ミッションモード
#include "script/view.c"			// 表示処理
#include "script/grade.c"		// 段位
#include "script/ranking2.c"		// ランキング2
#include "script/ranking3.c"		// ランキング3
#include "readini.c"				// ini読み込み

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  メイン
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void gameMain(void) {
	int		i, y, temp;

	do
	{
#include "heboris.ini"				// hoge 設定を読み込む
#include "heboris_user.ini"			// ユーザー設定を読み込む #1.60c7l7

		LoadIniFile("heboris.ini");
		LoadIniFile("heboris_user.ini");
		LoadIniFile("heboris_cpp.ini");
		YGS2kInit();
		loopFlag = true;

		y = 0;
		restart = 0;

		// 文字列バッファの初期化
		for ( i = 0 ; i < STRING_MAX ; i ++ )
		{
			string[i] = new char[512];
		}

		initialize();


		loop {
			StopAllWaves();
			StopAllBGM();

			backupSetups();	// 設定内容をバックアップ #1.60c7o6
			domirror = 0;	// 鏡像を無効に

			if(GetFPS() != max_fps_2) SetFPS(max_fps_2);

			title();

#if		0
			if(restart) {			// 再起動 こんなんでいいのかな? (^^; #1.60c5
				CallScript("script/GAMEINIT.C");
				printFont(1, 1, "PLEASE WAIT...", 2);
				halt;
				JumpScript("script/GAMESTART.C");
				return;
	//			goto reload;
			}
#else
			if(restart) {			// 再起動
				loopFlag = false;
				break;
			}
#endif

			flag = 0;
			loop {
	//			spriteTime();
				KeyInput();

				// リプレイ中の早送り
				noredraw = 0;
				if(playback && !demo) {
					playback = 0;
					if(getPushState(0, 4) || getPushState(1, 4)) fast = 5;
					if(!(getPressState(0, 4) || getPressState(1, 4))) fast = 0;
					if(!fast && (getPressState(0, 0) || getPressState(1, 0))) fast = 3;
					if(!fast && (getPressState(0, 1) || getPressState(1, 1))) fast = 1;
					if(getPushState(0, 5) || getPushState(1, 5)) {
						if(!tmp_maxPlay)
							maxPlay = 0;
						restoreSetups();
						if(gameMode[0] == 8) gameMode[0] = 0;
						if(gameMode[0] == 4){
							gameMode[0] = 0;
							gameMode[1] = 0;
						}
						flag = 1;
						break;
					}
					if(getPushState(0, 7))
						dispLinfo = !dispLinfo;
					playback = 1;
				}

				count = (count + 1) % 65536;

				// 背景非表示設定追加 #1.60c7i9
				if(!skip_viewbg) viewBack();

				viewFldBackground();

				// スタッフロール #1.60c7n5
				for(i = 0; i < fast + 1; i++) {
					staffRoll(0);
					staffExecute(0);

					if(maxPlay) {
						staffRoll(1);
						staffExecute(1);
					}
				}

				viewField();

				for(i = 0; i < fast + 1; i++) {
					playerExecute();


					// 演出無し設定追加 #1.60c7i9
					if((i > 0) && (!skip_obj)) objectExecute();

					noredraw = 1;
				}

				viewPreview();

				// スコア・タイム表示無し設定追加 #1.60c7i9
				if(!skip_viewscore) viewScore();

				// 演出無し設定追加 #1.60c7i9
				if(!skip_obj) objectExecute();


				lastProc();

				spriteTime();

				// flagが1以上になるとタイトルへ
				if(flag > 0) {
					hnext[0] = dispnext;
					hnext[1] = dispnext;
					break;
				}
			}
		}
		YGS2kExit();
	} while ( restart );
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  最後にやる処理
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void lastProc(void) {
	int		i,j,tmp,pl,fadec;

	if((playback) && (!demo)){
		if(gameMode[0] != 4){
			ExBltRect(87,29 + 128 * maxPlay + 213 * ((gameMode[0] == 8) && (!maxPlay)), 65, (time2[0]%6)*48 ,(time2[0]%30/6)*48,48,48);
			ExBltRect(85,34 + 128 * maxPlay + 213 * ((gameMode[0] == 8) && (!maxPlay)), 73, 35, 7*6, 22, 7);//ROTSの文字
			ExBltRect(55,22 + 128 * maxPlay + 213 * ((gameMode[0] == 8) && (!maxPlay)), 84 - (3 * (getDrawRate() == 1)), 64*rots[0] ,32*fontc[rots[0]] + (7 * (getDrawRate() == 1)),64,12 + (5 * (getDrawRate() == 1)));
			if(fast != 0){
				if(((!gflash[0]) || (enable_grade[0] != 4) || (gameMode[0] > 2)) && (!tspin_c[0])){
					printFont(14 - 12 * maxPlay, 26, "FF", 4);
					for(i = 0; i < fast + 1; i++)
						printFont(17 + i - 12 * maxPlay, 26, ">", 4);
					sprintf(string[0], "e%d", fast + 1);
					printFont(18 + i - 12 * maxPlay, 26, string[0], 4);
				}
			}else if(((!gflash[0]) || (enable_grade[0] != 4) || (gameMode[0] > 2)) && (!tspin_c[0])){
				printFont(17 - 12 * maxPlay, 26, "REPLAY", 4 * ((count % 80) >= 20));
			}
		}else{
			if(fast != 0){
				printFont(14, 26, "FF", 4);
				for(i = 0; i < fast + 1; i++)
					printFont(17 + i , 26, ">", 4);
				sprintf(string[0], "e%d", fast + 1);
				printFont(18 + i, 26, string[0], 4);
			}else printFont(17, 26, "REPLAY", 4 * ((count % 80) >= 20));
		}
	}
	if(demo) {
		if(maxPlay) {
			printFont(18, 17, "DEMO", 1);
		} else {
			// 一人しかいない場合はプレイ画面が見えにくいので修正#1.60c6.2d
			if(((count % 80) >= 20) && (!tspin_c[0]) && (sgrade[0] < min_sgrade) && (gflash[0] == 0))
				printFont(18 - 12 * maxPlay, 26, "DEMO", 1);
			ExBltRect(55,31 , 35, 64*rots[0] ,32*fontc[rots[0]],64,32);

			if(!english)
				ExBltRect(55,31, 69, 64*rots[0] ,320,64,32);
			else
				ExBltRect(55,31, 69, 64*rots[0] ,352,64,32);
			ExBltRect(55,31, 102 ,64*rots[0],416,64,32);
			showControl();
		}
		ExBltRect(87,136, 140, (count%6)*48 ,(count%30/6)*48,48,48);

		ExBltRect(77, 0, 176,  count % 320, 20, 320 - (count % 320), 8);
		ExBltRect(77, 320 - (count % 320), 176,  0, 20, count % 320, 8);

		ExBltRect(77, count % 320, 144,  0, 28, 320 - (count % 320), 8);
		ExBltRect(77, 0, 144, 320 - (count % 320), 28, count % 320, 8);

		if(count % 80 < 40) {
			ExBltRect(81,53,154,0,204,213,19);
		}

		if(getDrawRate() != 1) ExBltRect(3,101,120 - 23 * maxPlay,99,83,118,29);

		demo = 0;
		cpu_flag[0] = 0;
		cpu_flag[1] = 0;

		if(getPushState(0, 4) || getPushState(1, 4)) {
			StopAllBGM();
			StopSE(40);
			cpu_flag[0] = 0;
			cpu_flag[1] = 0;
			rots[0] = setupBak[4] % 10;
			rots[1] = setupBak[4] / 10;
			if(gameMode[0] == 4){
				gameMode[0] = 0;
				gameMode[1] = 0;
			}
			if(!tmp_maxPlay)
				maxPlay = 0;
			flag = 1;
		}

		if( (time[0] > 5400) || (time[1] > 5400) || (stat[0] == 14) || (stat[1] == 14)) {
			StopAllBGM();
			StopSE(40);
			cpu_flag[0] = 0;
			cpu_flag[1] = 0;
			rots[0] = setupBak[4] % 10;
			rots[1] = setupBak[4] / 10;

			if(ranking_type==0){//demotimeはデモ表示回数
				RankingProc_1(demotime);
			}else if(ranking_type==1){
				RankingProc_2(demotime);
			}else{
				RankingProc_3(demotime,0);
			}

			if(gameMode[0] == 4){
				gameMode[0] = 0;
				gameMode[1] = 0;
			}
			if(!tmp_maxPlay)
				maxPlay = 0;
			flag = 1;
		}

		demo = 1;
		cpu_flag[0] = 1;
		cpu_flag[1] = 1;
	}
	// 操作状況を表示するお遊び機能追加 #1.60c7f6
	else if(((playback) || (showctrl)) && (showctrl != 2) && (gameMode[0] != 4))
		showControl();

	// ポーズキーをjoypadに割り当て 1.60c7g7
	if(!playback) {
		if(getPressState(0, 9)) mpc4[0]++;
		else mpc4[0] = 0;
	} else {
		mpc4[0] = 0;
	}

	if((IsPushKey(pausekey[0]) || (mpc4[0] == 1))) {
		if( ((stat[0] >= 3) && (stat[0] <= 8) && (stat[0] != 7)) || (stat[0] == 13) || (stat[0] == 15) ||
			(stat[0] == 22) || ((stat[0] >= 25) && (stat[0] != 30) && (stat[0] != 36)) || (debug) ) {
			if(pause[0]) {
				// ポーズ解除
				pause[0] = 0;

				if(gameMode[0] == 0)
					ReplayWave(57);
				else
					ReplayWave(56);
			} else {
				// ポーズ
				pause[0] = 1;

				if(gameMode[0] == 0)
					PauseWave(57);
				else
					PauseWave(56);
			}

			if(gameMode[0] == 4)
				pause[1] = !pause[1];
		}
	}

	if(!playback) {
		if(getPressState(1, 9)) mpc4[1]++;
		else mpc4[1] = 0;
	} else {
		mpc4[0] = 0;
	}

	if((IsPushKey(pausekey[1]) || (mpc4[1] == 1))) {
		if( ((stat[1] >= 3) && (stat[1] <= 8) && (stat[1] != 7)) || (stat[1] == 13) || (stat[1] == 15) || (debug) ) {
			if(pause[1]) {
				// ポーズ解除
				pause[1] = 0;

				if(gameMode[1] == 0)
					ReplayWave(57);
				else
					ReplayWave(56);
			} else {
				// ポーズ
				pause[1] = 1;

				if(gameMode[1] == 0)
					PauseWave(57);
				else
					PauseWave(56);
			}

			if(gameMode[1] == 4)
				pause[0] = !pause[0];
		}
	}
	// TOMOYO E-Heart最終面ギミック C7U0
	for(pl = 0; pl <= maxPlay ; pl++){
	if((tomoyo_domirror[pl]) && (stat[1-pl] == 0)){
		SwapToSecondary(23);
		ExBltFastRect(23, 160*pl, 0, 160*pl, 0, 160, 240);
		SwapToSecondary(23);
		ExBltFastRect(23, 160*(!pl), 0,160*pl,0,160,240);
		if((ending[pl] != 3) && (stat[pl] != 21) && (stat[pl] != 20)){
			if(tomoyo_ehfinal_c[pl] < 220)
				fadec = 19;
			else if((tomoyo_ehfinal_c[pl] >= 220) && (tomoyo_ehfinal_c[pl] < 240))
				fadec = 240-tomoyo_ehfinal_c[pl];
			else if((tomoyo_ehfinal_c[pl] >= 240) && (tomoyo_ehfinal_c[pl] < 460))
				fadec = 0;
			else
				fadec = tomoyo_ehfinal_c[pl]-460;

			for(tmp = 0; tmp <= 1 ; tmp++)
				for(i = 0; i < 20; i++)
					for(j = 0 + 4 * pl; j < 10 + 4 * pl; j++)
						ExBltRect(75, (24+(160*tmp))+(j * 8), (i+1+ 4) * 8, tmp*8, fadec*8, 8, 8);
		}
	} else if((tomoyo_domirror[pl]) && (ending[pl] != 3)){
		if(tomoyo_ehfinal_c[pl] < 220)
			fadec = 19;
		else if((tomoyo_ehfinal_c[pl] >= 220) && (tomoyo_ehfinal_c[pl] < 240))
			fadec = 240-tomoyo_ehfinal_c[pl];
		else if((tomoyo_ehfinal_c[pl] >= 240) && (tomoyo_ehfinal_c[pl] < 460))
			fadec = 0;
		else
			fadec = tomoyo_ehfinal_c[pl]-460;

		tmp = pl;
		for(i = 0; i < 20; i++)
			for(j = 0 + 4 * pl; j < 10 + 4 * pl; j++)
				ExBltRect(75, (24+(160*tmp))+(j * 8), (i+1+ 4) * 8, tmp*8, fadec*8, 8, 8);
	}
	}
	if(thunder_timer){
		i = (10 - Rand(20))*getDrawRate();
		j = (10 - Rand(20))*getDrawRate();
		SetSecondaryOffset(i-(i/2),j-(j/2));
		thunder_timer--;
	}else{
		SetSecondaryOffset(0,0);
	}

	/* FPS表示 */
	// heboris.iniの設定でFPSを非表示にできる#1.60c7e
	if(!hide_fps) {
		sprintf(string[0], "%3d/%2dFPS", GetRealFPS(), GetFPS() );	// FPSの取得(測定値)
		printTinyFont(130, 233, string[0]);
	}

	// NEXT隠し
	if(IsPushKey(dispnextkey[0]) && (!demo) && (!playback) && (!death_plus[0]) && (!hebo_plus[0])&&(!heboGB[0])&&(!onRecord[0])) {
		// next表示個数は0～6 #1.60c7q3
		hnext[0]++;
		if(hnext[0] > 6) hnext[0] = 0;
		if((hnext[0] > max_hnext[0]) && onRecord[0]) max_hnext[0] = hnext[0];
	}
	if(IsPushKey(dispnextkey[1]) && (!demo) && (!playback) && (!death_plus[1]) && (!hebo_plus[1])&&(!heboGB[1])&&(!onRecord[1])) {
		hnext[1]++;
		if(hnext[1] > 6) hnext[1] = 0;
		if((hnext[1] > max_hnext[1]) && onRecord[1]) max_hnext[1] = hnext[1];
	}

	if(flag || demo) {
		pause[0] = 0;
		pause[1] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  タイトル
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void title(void) {
	int		player, ofs, pl, mode, game, i, democ;

	// 初期設定
	count = 0;
	democ = 0;
	ofs = 0;
	pl = 1;
	mode = 0;
	game = 0;
	title_acnt = 0;

	demo = 0;
	playback = 0;
	cpu_flag[0] = 0;
	cpu_flag[1] = 0;

	tmp_maxPlay = maxPlay;	// プレイ人数をバックアップ

	// 画面位置修正 via C++ Port
	SetSecondaryOffset(0,0);

	if(wavebgm == 0) {	// No.30→38に変更 #1.60c7i2
		if(!IsPlayMIDI()) PlayMIDI();
	} else if(wavebgm >= 1) {
		if(!IsPlayWave(61)) PlayWave(61);
	}


	// ループ開始
	loop {
		count++;
		democ++;

		// ｷｰ入力
		KeyInput();

		// 背景を描く
		if(!title_mov_f){
			ExBltFast(8, 0, 0);
		} else {
			ExBltFastRect(8,0,0,(title_acnt / 10) * 320,(title_acnt % 10) * 240, 320, 240);
			if(count % mov_inter == 0)
				title_acnt++;
			if(title_acnt > title_mov_f -1)
				title_acnt = 0;
		}
		ExBltRect(77, 0, 106,  count % 320, 4, 320 - (count % 320), 8);
		ExBltRect(77, 320 - (count % 320), 106,  0, 4, count % 320, 8);

		ExBltRect(77, count % 320, 20,  0, 4, 320 - (count % 320), 8);
		ExBltRect(77, 0, 20, 320 - (count % 320), 4, count % 320, 8);


		ExBltRect(77, 0, 176 + ofs,  count % 320, 20, 320 - (count % 320), 8);
		ExBltRect(77, 320 - (count % 320), 176 + ofs,  0, 20, count % 320, 8);

		ExBltRect(77, count % 320, 144 - (ofs / 5),  0, 28, 320 - (count % 320), 8);
		ExBltRect(77, 0, 144 - (ofs / 5), 320 - (count % 320), 28, count % 320, 8);

		ExBlt(7, 0, 0);

		// 著作権表示
		printFont(6, 28, "a 1998-2002 KENJI HOSHIMOTO", 0);

		// バージョン表示
		printFont(27, 13, "VERSION 1.60", 4);
		printFont(27, 14, "(2002/03/31)", 6);
		sprintf(string[0], "%20s", version);
		printFont(20 - StrLen(version) / 2, 16, string[0], 1); // #1.60c7f4

		// モード0: ボタン入力待ち
		if(mode == 0) {
			if(ofs > 0) ofs = ofs - 2;
			ExBltRect(87,136, 140, (count%6)*48 ,(count%30/6)*48,48,48);
			// スタートボタンを押してください
			if(count % 80 < 40) {
				ExBltRect(81,53,154,0,204,213,19);
			}

			// どちらかのスタートボタンが押されたらモード1へ
			for(player = 0; player < 2; player++) {
				if(getPushState(player, 4)) {
					democ = 0;
					PlaySE(10);
					mode = 1;
				}
			}

			// HOLDボタンでサウンドテスト#1.60c7c
			if(getPushState(0, 7))
				SoundTestProc();

			// Cボタンでテストメニュー#1.60c7i4
			if(getPushState(0, 6)) {
				testmenu();
				if(restart) mode = 2;
			}

			// 入力待ちの時だけデモ画面へ#1.60c7g4
			if(democ > 1000) {
				doDemoMode();
				mode = 2;
			}
		} else if(mode == 1) {
			// モード1: モードセレクト
			democ = 0; // デモ待ち時間リセット #1.60c7g4
			if(ofs < 40) ofs = ofs + 2;
			printFont(14, 18 + game, "b", fontc[rots[0]]);

			// モード一覧表示
			printFont(15, 18, "SOLO MODE",            (fontc[rots[0]]) * (game == 0));
			printFont(15, 19, "VERSUS MODE",          (fontc[rots[0]]) * (game == 1));
			printFont(15, 20, "PRACTICE MODE",        (fontc[rots[0]]) * (game == 2));
			printFont(15, 21, "MISSION MODE",         (fontc[rots[0]]) * (game == 3));
			printFont(15, 22, "REPLAY",               (fontc[rots[0]]) * (game == 4));
			printFont(15, 23, "NORMAL RANKING",       (fontc[rots[0]]) * (game == 5));
			printFont(15, 24, "SECTION TIME RANKING", (fontc[rots[0]]) * (game == 6));
			printFont(15, 25, "SETTING",              (fontc[rots[0]]) * (game == 7));
			printFont(15, 26, "OPTION",               (fontc[rots[0]]) * (game == 8));

			// どちらかのジョイスティックでモードセレクト
			for(player = 0; player < 1+maxPlay; player++) {
				// 押しっぱなし移動 #1.60c7n4
				padRepeat2(player);

				// 上へ
				if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
				if(getPressState(player, 0)) {
					PlaySE(5);
					game--;
					if(game < 0) game = 8; // 対戦モード追加に従い修正 #1.60c7g1
				}

				// 下へ
				if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
				if(getPressState(player, 1)) {
					PlaySE(5);
					game++;
					if(game > 8) game = 0; // 対戦モード追加に従い修正 #1.60c7g1
				}

				// キャンセル
				if(getPushState(player, 5)) {
					democ = 0;
					mode = 0;
					domirror = 0;
				}

				// 決定
				if(getPushState(player, 4)) {
					PlayWave(10);
					mode = 2;
					domirror = 0;	//鏡像は最初は無効、必要に応じて1にする

					// ソロモード
					if(game == 0) {
						enterSoloMode(player);
						break;
					// 対戦モード #1.60c7g1
					} else if(game == 1) {
						tmp_maxPlay = maxPlay;	// プレイ人数をバックアップ
						if(!tmp_maxPlay) {
							maxPlay = 1;
						}
						if(def_vs_goal == 0)
							vs_goal = 0;
						else if(def_vs_goal / 50 == 0)
							vs_goal = def_vs_goal;
						else
							vs_goal = 200;
						upLineT[0] = def_vs_uplinetype;
						upLineT[1] = def_vs_uplinetype;
						use_item[0] = 0;
						use_item[1] = 0;
						enterVersusMode(player);
						setStartLevel(0);
						setStartLevel(1);
						break;
					// プラクティスモード
					} else if(game == 2) {
						enterPracticeMode(player);
						break;
					// ミッションモード
					} else if(game == 3) {
						enterMissionMode(player);
						break;
					// リプレイ
					} else if(game == 4) {
						domirror = 0;	// 直後に選択画面に移行するため、鏡像を無効
						mode = ReplaySelectProc();
						game = 0;
						ofs = 0;
					// ランキング
					} else if(game == 5) {
						rankingmode = 0;
						if(ranking_type==0){
							RankingProc2_1();
						}else if(ranking_type==1){
							RankingProc2_2();
						}else{
							RankingProc2_3();
						}
						game = 0;
						mode = 0;
						ofs = 0;
					// セクションタイムランキング
					} else if(game == 6) {
						ST_RankingView();
						game = 0;
						mode = 0;
						ofs = 0;
					// 設定
					} else if(game == 7) {
						ConfigMenu();
						game = 0;
						ofs = 0;
						if(restart) mode = 2;
						else mode = 0;
					// オプション #1.60c7n4
					} else if(game == 8) {
						GameOption();	// config.c
						game = 0;
						mode = 0;
						ofs = 0;
					}
				}
			}
		} else {
			// モード2: ループから抜ける
			break;
		}
		spriteTime();
	}
}

/* SOLO MODEに入る */
void enterSoloMode(int player) {
	hnext[0] = dispnext;
	hnext[1] = dispnext;

	domirror = 1;	// 鏡像を有効
	onRecord[0] = 0;
	onRecord[1] = 0;

	randommode[0] = 1;
	versusInit(0);	// 1P NEXTを初期化
	randommode[0] = 0;
	if(maxPlay){
		randommode[1] = 1;
		versusInit(1);	// 2P NEXTを初期化 #1.60c7m1
		randommode[1] = 0;
	}
	p_monochrome_mode=0;
	gameAllInit();	// ゲーム全体初期化

	// SHIRASE関係#1.60c7f4
	level_shirase_start = p_shirase_start_level;
	raise_shirase_lines = p_shirase_line;
	raise_shirase_interval = p_shirase_interval;

	stat[player] = 1;					// ブロックシャッター実行
	statc[player * 10] = 0;				// ステータスカウンタを0に
	statc[player * 10 + 1] = 2;			// シャッター後はステータスNo.2(SOLO MODEモードセレクト)
	statc[player * 10 + 4] = 16;
	// NEXTブロックの色を設定 #1.60c7n1
}

/* VERSUS MODEに入る */
void enterVersusMode(int player) {
	int i;

	hnext[0] = dispnext;
	hnext[1] = dispnext;
	onRecord[0] = 0;
	onRecord[1] = 0;
	gameMode[0] = 4;
	gameMode[1] = 4;
	gameAllInit();	// ゲーム全体初期化

	backno = 61;

	// SHIRASE関係#1.60c7f4
	level_shirase_start = p_shirase_start_level;
	raise_shirase_lines = p_shirase_line;
	raise_shirase_interval = p_shirase_interval;
	p_monochrome_mode=0;

	for(i=0;i<2;i++){
		vslevel[i] = 0;
		gameMode[i] = 4;				// VSモード

		stat[i] = 1;					// ブロックシャッター実行
		statc[i * 10] = 0;				// ステータスカウンタを0に
		statc[i * 10 + 1] = 16;			// シャッター後はステータスNo.16
		if(versus_rot[i] != 9)
			rots[i] = versus_rot[i];
		vs_points[i] = 0;
	}
	versusInit(0);	// NEXT初期化
	if((cp_player_1p) && (cp_player_2p)){
		versusInit(1);
	}else{
		// 1PのNEXTを2Pにコピー #1.60c7m1
		for(i=0;i<1400;i++) {
			nextb[i + 1400] = nextb[i];
		}
		next[1] = next[0];
	}
	nextc[0] = 0;
	nextc[1] = 0;
//	next[1] = next[0];
	if(vs_time != 0) {
		ltime[0] = vs_time;
		ltime[1] = vs_time;
	}
	// NEXTブロックの色を設定 #1.60c7n1
	setNextBlockColors(0, 1);
	setNextBlockColors(1, 1);
}

/* PRACTICEモードに入る */
void enterPracticeMode(int player) {
	hnext[0] = dispnext;
	hnext[1] = dispnext;

	gameMode[0] = 5;				// PRACTICEモード 先にセット #1.60c7g3
	gameMode[1] = 5;
	onRecord[0] = 0;
	onRecord[1] = 0;

	bakhold = p_hold;				// #1.60c7j5

	p_nextblock = nextblock;

	versusInit(0);	// NEXT初期化
	if(maxPlay) versusInit(1);

	gameAllInit();

	// PRACTICEでのせり上がり開始レベルは0 #1.60c7i2
	level_shirase_start = 0;
	raise_shirase_lines = p_shirase_line;
	raise_shirase_interval = p_shirase_interval;

	backno = p_backno;

	vslevel[0] = 0;
	vslevel[1] = 0;

	stat[0] = 1;					// ブロックシャッター実行
	statc[0 * 10] = 0;				// ステータスカウンタを0に
	statc[0 * 10 + 1] = 9;			// シャッター後はステータスNo.9

	lvup[0] = 1;					// レベルタイプ(1p) #1.60c7i2
	lvup[1] = 1;					// レベルタイプ(2p) #1.60c7i2

	stat[1] = 10;					// 2PはステータスNo.10 (WAIT)

	loadWait(0, def_p_sp);					// デフォルト設定読み込み

	// バックアップにデフォルト設定を入れる
	b_wait1[0] = wait1[0];
	b_wait2[0] = wait2[0];
	b_wait3[0] = wait3[0];
	b_waitt[0] = waitt[0];
	b_sp[0]    = sp[0];
}

/* MISSION MODEに入る */
void enterMissionMode(int player) {
	gameMode[0] = 8;

	hnext[0] = dispnext;
	hnext[1] = dispnext;

	domirror = 0;
	onRecord[0] = 0;
	onRecord[1] = 0;

	versusInit(0);	// 1P NEXTを初期化
	p_over1000=0;
	gameAllInit();	// ゲーム全体初期化

	stat[0] = 1;					// ブロックシャッター実行
	statc[0 * 10] = 0;				// ステータスカウンタを0に
	statc[0 * 10 + 1] = 23;			// シャッター後はステータスNo.23(MISSION MODEモードセレクト)

	stat[1] = 10;					// 2PはステータスNo.10 (WAIT)

	// デフォルトの問題データファイルを読み込み
	loadMissionData(mission_file);

	c_mission = 0;
	c_norm[0] = 0;
	c_norm[1] = 0;
	clear_mission = 0;
	eraser_now_lines = 0;

	vslevel[0] = 0;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  デモ画面準備
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void doDemoMode(void) {
	int		player,tmp;

	demo = 1;
	demotime = (demotime + 1) & 3;

	rots[0] = Rand(9);
	rots[1] = Rand(9);
	if(Rand(10) < 6){	//VSデモ
		gameMode[0] = 4;
		gameMode[1] = 4;
		tmp_maxPlay = maxPlay;	// プレイ人数をバックアップ
		maxPlay = 1;
	}
	else{	//通常デモ
		for(player = 0; player <= maxPlay; player++) {
			do{
				tmp = Rand(11);
			}while((tmp == 2) || (tmp == 3) || (tmp == 4) || (tmp == 5) || (tmp == 6) || (tmp == 8) || (tmp > 10));
			gameMode[player] = tmp;
			if(gameMode[player] == 9){
				std_opt[player] = Rand(2);
				relaymode[player] = Rand(2);
			}else if(gameMode[player] == 10){
				ori_opt[player] = Rand(2);
			}
			if(gameMode[player] == 1) item_mode[player] = (Rand(10) < 4);
		}
		backno = Rand(12);
		bgmlv = 0;
	}

	gameAllInit();
	versusInit(0);
	versusInit(1);

	if(gameMode[0] == 4){
		backno = 61;
		bgmlv = 10;
	}
	else{
		backno = Rand(12);

		bgmlv = setstartBGM(gameMode[0], 0);
		fadelv[0] = 0;
		fadelv[1] = 0;
	}
	for(player = 0; player <= maxPlay; player++) {
		cpu_flag[player] = 1;				// CPU操作フラグON
		stat[player] = 1;					// ブロックシャッター実行
		statc[player * 10] = 0;				// ステータスカウンタを0に
		statc[player * 10 + 1] = 3;			// シャッター後はステータスNo.3
	}
}

void playerInitial(int player) {
	int		i, j;

	sc[player] = 0;
	lv[player] = 0;
	li[player] = 0;
	lc[player] = 0;
	sp[player] = 1;
	time[player] = 0;
	timeOn[player] = 0;

	skill[player] = 0;	// 961追加
	start_flag[player] = 1;	// 961追加
	for(i = 0; i < 100; i++) {
		lap_time[i + player * 100] = 0;	// #1.60c7j7追加
		split_time[i + player * 100] = 0;	// #1.60c7j7追加
	}
	for(i = 0;i<30;i++){
		stage_time[i + player * 30] = 0;
		st_other[i + player * 30] = 0;		//落としたブロック数
	}

	for(i = 0;i<20;i++){
		lvstop_time[i + player * 20] = 0;
	}

	if(gameMode[player]!= 5){
		nextc[player] = 0;
		next[player] = nextb[0 + player * 1400];
	}else{
		nextc[player] = b_nextc[player];
		next[player] = nextb[b_nextc[player] + player * 1400];
	}
	//hold[player] = -1;				// hold初期化	#1.60c
	if (gameMode[player] == 5) hold[player] = bakhold;
	else hold[player] = -1; // hold初期化 #1.60c

	hold_used[player] = 0;			// HOLDした回数 #1.60c7i1

	kickc[player] = 0;	// 永久床蹴り防止用 #1.60c3
	kickc2[player] = 0;
	tc[player] = -1;		// -1に変更(表示上は0)#1.60c7i8

	tr[player] = 0;			// tgmRank	#1.60c3
	start[player] = 0;		// ゲーム開始レベル	#1.60c3
	hidden[player] = 0;
	hiddenti[player] = 0;
	hiddentm[player] = 0;
	hiddeny[player] = 22;
	hiddenti[player] = 0;
	hiddenc[player] = 0;
	upLineNo[player] = 0;	// せり上がりラインNo	#1.60c6.2g

	// せり上がり設定を反映#1.60c6.2f
	shirase[player] = p_shirase_interval;
	bdowncnt[player] = 0;	// ブロックを着床させた回数 #1.60c7i2
	st_bdowncnt[player] = 0;
	erasecnt[player] = 0;

	// フィールドサイズなど初期化、BIG時などは上書きする事 #1.60c7
	fldsizew[player] = 10;
	fldsizeh[player] = 21;
	BigMove[player] = bigtype;	//#1.60c7m6
	IsBig[player] = 0;
	IsBigStart[player] = 0;

	wait1[player] = 26;
	wait2[player] = 40;
	wait3[player] = 28;
	waitt[player] = 15;
	combo[player] = 0;
	comboM[player] = 0;
	combo2[player] = 0;
	hebocombo[player] = 0;		// #1.60c7k4
	pinch[player] = 0;
	pinch2[player] = 1;
	pass_flg[player] = 0;		// #1.60c7j5
	isrollroll[player] = 0;		// #1.60c7j5
	isxray[player] = 0;			// #1.60c7p9ex
	xray_counter[player] = 0;	// #1.60c7p9ex
	iscolor[player] = 0;		// #1.60c7p9ex
	color_counter[player] = 0;	// #1.60c7p9ex

	if(gameMode[player] != 5) {
		// #1.60c7j5
		fmirror = 0;
		rollroll = 0;
		shadow_timer = 0;
		squaremode[player]=0;

		// #1.60c7p9ex
		xray = 0;
		fcolor = 0;
		thunder = 0;
	}
	isfmirror[player] = 0;		// #1.60c7l8
	rollroll_flag[player] = 0;	// #1.60c7l8
	xray_flag[player] = 0;		// #1.60c7p9ex
	color_flag[player] = 0;		// #1.60c7p9ex

	isrotatelock[player] = 0;		//回転封じ
	isholdlock[player] = 0;			//HOLD封じ
	istimestop[player] = 0;			//時間停止
	ishidenext[player] = 0;			//NEXT隠し
	ismagnetblock[player] = 0;		//マグネット
	isLRreverse[player] = 0;		//左右逆転
	item_timer[player] = 0;
	stopmirror_flag[player] = 0;
	isUDreverse[player] = 0;		//上下逆転
	isboost[player] = 0;			//ブーストファイア
	isfever[player] = 0;			//フィーバー
	isremote[player] = 0;			//リモートコントロール
	isdark[player] = 0;				//ダーク
	isUPdelfield[player] = 0;		//↑DEL FIELD
	isDWdelfield[player] = 0;
	isdeleven[player] = 0;
	isreflect[player] = 0;
	istrance[player] = 0;
	isdouble[player] = 0;
	ismiss[player] = 0;
	isallclear[player] = 0;
	isfakenext[player] = 0;

	isblind[player] = 0;			//ブラインドON/OFF制御
	blind_flag[player] = 0;			//ブラインドの有無
	isthunder[player] = 0;
	thunder_timer = 0;

	tomoyo_rise[player] = 0;
	tomoyo_rise_t[player] = 0;
	hnext_flag[player] = 0;
	hnext_timer[player] = 0;
	tomoyo_waits[player] = 0;
	tomoyo_domirror[player] = 0;
	tomoyo_ehfinal_c[player] = 0;
	fpbas_mode[player] = 0;

	item_mode[player] = 0;
	hebo_plus[player] = 0;

	stat[player] = 0;

	bgmlv = 0;
	fadelv[player] = 0;
	anothermode[player] = 0;
	ending[player] = 0;
	edrec[player] = 0;

	// 段位 #1.60c7g5
	grade[player] = 0;
	gmflag1[player] = 0;
	gmflag2[player] = 0;
	gmflag1_e[player] = 0;
	gmflag2_e[player] = 0;
	gmflag_r[player] = 0;	// #1.60c7i2
	semigmflag_r[player] = 0;
	gmflag3[player] = 0;
	gmflag3_e[player] = 0;

	// #1.60c7g7
	timelimit[player] = 0;
	timelimitw[player] = 0;
	timelimitm[player] = 0;
	timelimitmw[player] = 0;
	timelimit2[player] = 0;
	// #1.60c7k1
	shadow_timer_flag[player] = 0;

	// #1.60c7k4
	allclear[player] = 0;

	// #1.60c7l5
	stage[player] = 0;			// ステージ番号
	stime[player] = 60 * 60;	// ステージタイム
	ltime[player] = 180 * 60;	// リミットタイム
	ctime[player] = 0;			// クリアタイム

	// #1.60c7l7
	rest_pblock[player] = 0;

	// #1.60c7l8
	sclear[player] = 0;

	// #1.60c7m3
	clearnum[player] = 0;		//クリアステージ数
	clearp[player] = 0;			//クリア率

	// #1.60c7m9
	c_cblk[player] = 0;
	c_cblk_r[player] = 0;
	c_hblk[player] = 0;
	setNextBlockColors(player, 1);

	// #1.60c7n1
	item[player] = 0;
	item_name[player] = 0;
	item_g[player] = 0;
	item_t[player] = 0;
	item_rollroll[player] = 0;
	item_monochrome[player] = 0;
	// #1.60c7n3
	rescue[player] = 0;
	REflag[player] = 0;

	// #1.60c7n6
	tspin_c[player] = 0;
	tspin_flag[player] = 0;

	// #1.60c7n8
	replay_save[player] = 1;
	time2[player] = 0;

	// #1.60c7o1
	sgrade[player] = -1;

	// #1.60c7o2
	cpu_flag[player] = 0;

	// #1.60c7o6
	endtime[player] = 0;

	// #1.60c7o7
	t_training[player] = 0;

	stp[player] = 0;
	stg[player] = 0;
	sts[player] = 0;
	stb[player] = 0;

	// #1.60c7q5
	item_nblk[0 + player * 6] = 0;
	item_nblk[1 + player * 6] = 0;
	item_nblk[2 + player * 6] = 0;
	item_nblk[3 + player * 6] = 0;
	item_nblk[4 + player * 6] = 0;
	item_nblk[5 + player * 6] = 0;
	item_coming[player] = 0;
	item_waiting[player] = 0;
	hanabi_frame[player] = 0;
	hanabi_waiting[player] = 0;
	// #1.60c7q7
	hanabi_flag = 0;
	hanabi_combo[player] = 0;
	hanabi_total[player] = 0;
	endingcnt[player] = 0;
	tomoyo_hanabi[player] = 0;
	itemappear_c[player]=0;

	laststage[player] = 26;		// laststageをplayerごとに #1.60c7r7

	// #1.60c7r9
	c_mission = 0;
	c_norm[player] = 0;
	for(i = 0; i < 4; i++) cycle[i] = 0;

	// #1.60c7s4
	clear_mission = 0;
	eraser_now_lines = 0;

	// #1.60c7s6
	if(gameMode[player] != 4){
		upLineT[player] = uplinetype;
		use_item[player] = 0;
	}

	if(demo)
		upLineT[player] = 3;
	upLines[player] = 0;
	last_tspin[player]=0;
	b_to_b_flag[player]=0;
	b_to_b_c[player]=0;

	platinaerase_cnt[player]=0;
	ext_s[player]=0;
	timeextend_snd_cnt[player]=0;

	fmirror_cnt[player]=-20;

	staffroll_y_ex[player] = 0;

	tomoyo_opt[player] = 0;
	heboGB[player] = 0;

	gflash[player] = 0;		// 段位が上がった時に表示を光らせる時に使用
	gup_down[player] = 0;	//cool:1とregret:0
	// #1.60c7t5
	gpoint[player] = 0;//ポイント
	gtime[player] = 0;//タイム

	skillbai[player]=0;
	skillg[player] = 0;
	endlines[player] = 0;

	bgfade_cnt = 0;

	over1000_block[player] = 0;

	for(i=0;i <= 10;i++)
		p_erase_c[i] = 0;

	start_mission = 0;
	if(!playback)
		max_hnext[player] = hnext[player];
	death_plus[player] = 0;

	time99[player] = 0;
	timeN[player] = 0;
	ave_laptime3[player] = 0;
	qualityflag[player]= 0;
	skillflag[player] = 0;
	gup3rank[player] = 0;
	tr2[player] = 0;
	grade2[player] = 0;
	border_rank[player] = 0;

	min_speed[player] = 0;

	ndelay[player] = 1;
	navigation_time[player] = 0;

	novice_mode[player] = 0;
	novice_flag[player] = 0;

	// #1.60c7u0.5
	medal_ac[player] = 0;
	medal_sk[player] = 0;
	medal_co[player] = 0;
	medal_re[player] = 0;
	medal_st[player] = 0;
	end_f[player] = 0;

	lasernum[player] = 0;
	rapid_c[player] = 0;
	dolaser[player] = 0;
	do16t[player] = 0;

	for(i = 0; i < 4;i++)
		laserpos[i + 4 * player] = -1;

	donega[player] = 0;

	for(i = 0; i < 10; i++)
		nega_pos[i + 10 * player] = 0;

	doshotgun[player] = 0;

	for(i = 0; i < 22; i++)
		shotgunpos[i + 22 * player] = 0;

	doexchg[player] = 0;
	docopyfld[player] = 0;

	rt_nblk[0 + 6 * player] = 0;
	rt_nblk[1 + 6 * player] = 0;
	rt_nblk[2 + 6 * player] = 0;
	rt_nblk[3 + 6 * player] = 0;
	rt_nblk[4 + 6 * player] = 0;
	rt_nblk[5 + 6 * player] = 0;

	isbanana[player] = 0;
	for(i = 0; i < 10; i++)
		banana_pos[i + 10 * player] = 0;
	banana_c[player] = 0;

	examination[player] = 0;
	exam_grade[player] = 0;
	purupuru[player] = 0;

	stage_skip_mpc[player] = 0;
	hebop_interval[player] = 15;
	harddrop[player] = 0;

	tr_20g[player] = 0;

	dorulet[player] = 0;

	ace_bgmchange[player] = 0;

	are_skipflag[player] = 0;

	hanabi_int[player] = 20;

	stp_point[player] = 0;

	isFreefall[player] = 0;		// フリーフォール
	Ff_rerise[player] = 0;		// ↑の消去後再せり上げライン数
	isLmovfield[player] = 0;	// ←MOV FIELD
	isRmovfield[player] = 0;	// →MOV FIELD
	do180field[player] = 0	;	// 180°フィールド

	ofs_x[player] = 0;
	ofs_y[player] = 0;
	ofs_x2[player] = 0;
	ofs_y2[player] = 0;

	kickc3[player] = 0;

	regretcancel[player] = 0;
	upLines_waiting[player] = 0;

	devil_minus[player] = 0;

	devil_nextrise[player] = 0;

	FP_bonus[player] = 0;
	FP_bonus_c[player] = 0;
	fastroll[player] = 0;
	monochrome_mode[player] =0;
	p_monochrome_mode=0;
	std_opt[player] = 0;
	ori_opt[player] = 0;
	squarecnt[player] = 0;
	scrate[player]=100;

	relaymode[player] = 0;
	first_rot[player] = rots[player];
	relayround[player] = 0;

	itemhistory[ 0 + 5 * player ] = 0;
	itemhistory[ 1 + 5 * player ] = 0;
	itemhistory[ 2 + 5 * player ] = 0;
	itemhistory[ 3 + 5 * player ] = 0;
	itemhistory[ 4 + 5 * player ] = 0;

	item_num = 39;// アイテムの総数	アイテムを追加したら更新する事
					// 20:C7U0 最初  26:C7U1 レーザー、ネガフィールド、ショットガン、フィールド交換、
					// ハードブロック、シャッフルフィールド追加　27:ランダム追加　31:全部揃った

					//1:ミラー　2:rollroll 3:death 4:X-RAY 5:COLOR 6:ROTATELOCK 7:HIDENEXT 8:MAGNET 9:TIMESTOP 10:HOLDLOCK
					//11:→←REV 12:BOOST 13:FEVER 14:↑↓REV 15:REMOTE CON 16:DARK 17:↑DEL 18:↓DEL 19:DELEVEN 20:TRANSFORM
					//21:LASER 22:NEGA 23:SHOTGUN 24:EXCHG 25:HARD 26:SHUFFLE 27:RANDOM 28:FREEFALL 29:←MOV 30→MOV
					//31:180°32:16t 33:REFLECT 34:DOUBLE　35:ALLCLEAR 36:MISS 37:COPYFLD 38:FAKENEXT 39:[] //40:SPOTLIGHT
					//41:SPINFIELD 42:PRESSFIELD

	vs_style[player] = -1;
	vs_points[player] = 0;
	vs_round = 1;
	item_inter[player] = item_interval;
	if(item_interval <= 0) item_inter[player] = 1;

	misstimer[player] = 0;

	// 旧バージョンのリプレイデータを再生するためのスイッチ #1.60c7H2
	repversw = 65;	// 0:1.60c7f3以前 1:1.60c7f4以降 3:1.60c7g7以降 4:1.60c7h2以降 5:1.60c7h5以降 6:1.60c7i2以降
					// 7:1.60c7j3以降 8:1.60c7k5以降 9:1.60c7k8以降 10:1.60c7n6以降 11:1.60c7o9以降 12:1.60c7p1以降
					// 13:1.60c7p5以降 14:1.60c7q8以降 15:1.60c7r3以降 16:1.60c7r7以降 17:1.60c7s6以降 18:1.60c7t3以降
					// 19:C7T6.4以降 20:C7T6.8以降 21:C7T7.2以降 22C7T7.7以降 23:C7T8以降 25:C7T9.2以降 26:C7T9.4以降
					// 27:C7T9.5以降 28:C7U0以降 29:C7U0.5以降 30:C7U1以降 31:C7U1.3以降 32:C7U1.5以降 33:C7U1.7以降
					// 34:C7U2以降 35:C7U2.5以降 36:C7U2.8以降 37:C7U2.9以降 38:C7U3.7以降 39:C7U4.0以降 40:C7U4.3以降
					// 41:C7U4.5以降 42:C7U4.7以降 43:C7U4.72以降 44:C7U4.9以降 45:C7U5.0以降 46:C7U5.2以降 47:C7U5.45以降
					// 48:C7U5.3以降 48:C7U6以降	-中略-
					// 54:C7V2.1以降 55:C7V2.4以降 56:C7V2.43以降 57:C7V2.5以降 58:C7V3.3以降 59:C7V3.4以降 60:C7V3.45以降
					// 61:C7V3.55以降 62:C7V3.6以降 63:C7V3.65以降 64:C7V3.7以降 65:C7V3.75以降
	//SetVolumeWaveAll(0);
	item_pronum = 0;
	for(i = 0; i < 9; i++)
		relaydata[i + player * 9] = -1;

	for(i = 0 ; i < item_num ; i++){
		item_pronum = item_pronum + item_pro[i];
	}
	for(i = 0; i < 20; i++)
		st_new[i + player * 20] = 0;	// #1.60c7q7

	for(i = 0; i < 20; i++)
		repdata[i + player * 20] = -1;

	for(i = 0; i < 10; i++)
		statc[i + player * 10] = 0;

	for(i = 0; i < 22; i++)
		erase[i + player * 22] = 0;

	for(i = 0; i < 10; i++)
		for(j = 0; j < 22; j++) {
			fld[i + j * 10 + player * 220] = 0;
			fldt[i + j * 10 + player * 220] = 0;	// #1.60c7j5
			fldi[i + j * 10 + player * 220] = 0;	// #1.60c7n1
			fldu[i + j * 10 + player * 220] = 0;
			fldubuf[i + j * 10 + player * 220] = 0;
			flds[i + j * 10 + player * 220] = 0;
			fldsbuf[i + j * 10 + player * 220] = 0;
		}

}

// Bigモード用イニシアライズ
// …はとっくに不要になっていたので抹消


//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  初期化処理
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void gameAllInit(void) {
	backno = 0;
	fast = 0;
	count = 0;
	overcount = 0;

	bgfadesw = 0;
	bgfadelv = 0;

	playerInitial(0);
	playerInitial(1);

	if(!maxPlay)
		stat[1] = 0;

	objectClear();
	//staffInit2();	// #1.60c7n5
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  NEXT生成処理
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// 変数playerを追加しました。#1.60c7m1
void versusInit(int player) {
	int		i, j, k, same, temp, mae, shu, len;

	same = limit[nextblock];
	mae = -1;

	// ツモの読み込み #1.60c7g3
	len = 0;

	// tomoyoのパターン #1.60c7l9
	if( ((gameMode[player] == 6) && (!randommode[player])) || (nextblock ==11)|| ((p_nextblock ==11)&&(gameMode[player] == 5))) {
		if(start_stage[player] < 100){	//通常
			len = StrLen(nextb_list);
			if(len > 0) {
				for(i = 0; i < 1400; i++) {
					j = i % len;
					MidStr(nextb_list, j + 1, 1, string[0]);
					temp = ValLong(string[0]);
					if((temp >= 0) && (temp <= 6)) nextb[i + player * 1400] = temp;
				}
			}
		}else{							//F-Pointは電源パターン
			len = StrLen(nextfp_list);
			if(len > 0) {
				for(i = 0; i < 1400; i++) {
					j = i % len;
					MidStr(nextfp_list, j + 1, 1, string[0]);
					temp = ValLong(string[0]);
					if((temp >= 0) && (temp <= 6)) nextb[i + player * 1400] = temp;
				}
			}
		}
	} else if((nextblock == 9)|| ((p_nextblock ==9)&&(gameMode[player] == 5))) {
		// ガイドライン対応ゲーム風NEXT生成 #1.60c7s1(nextblock ==11)
		guidelineNextInit(player);
	} else if((nextblock == 8)|| ((p_nextblock ==8)&&(gameMode[player] == 5))) {
		// TGM風NEXT生成#1.60c7h4
		tgmNextInit(player);
	} else if((nextblock == 10)|| ((p_nextblock ==10)&&(gameMode[player] == 5))) {
		//電源パターンNEXT生成
		len = StrLen(nextdengen_list);
		//sprintf(string[0], "len=%d", len);
		if(len > 0) {
			for(i = 0; i < 1400; i++) {
				j = i % len;
				MidStr(nextdengen_list, j + 1, 1, string[0]);
				temp = ValLong(string[0]);
				if((temp >= 0) && (temp <= 6)) nextb[i + player * 1400] = temp;
			}
		}
	} else if((nextblock == 12)|| ((p_nextblock ==12)&&(gameMode[player] == 5))) {
		//FP電源パターンNEXT生成
		len = StrLen(nextfp_list);
		//sprintf(string[0], "len=%d", len);
		if(len > 0) {
			for(i = 0; i < 1400; i++) {
				j = i % len;
				MidStr(nextfp_list, j + 1, 1, string[0]);
				temp = ValLong(string[0]);
				if((temp >= 0) && (temp <= 6)) nextb[i + player * 1400] = temp;
			}
		}
	}else if((nextblock == 0)|| ((p_nextblock ==0)&&(gameMode[player] == 5))) {
		//完全ランダム
		do {
			nextb[0 + player * 1400] = Rand(7);
		} while((nextb[0 + player * 1400] != 0) && (nextb[0 + player * 1400] != 1) && (nextb[0 + player * 1400] != 4) && (nextb[0 + player * 1400] != 5));

		for(i = 1; i < 1400; i++) {
			nextb[i + player * 1400] = Rand(7);
		}
	} else {
		shu = nextblock;
		for(i = 0; i < 1400 / shu + 1; i++) {
			for(j = 0; j < 7; j++) nextwork[j] = 0;
			same = same - (same > 0);

			for(j = 0; j < shu; j++) {
				do {
					//初手
					if((shu * i + j == 0) && ( ((gameMode[player] != 5) && (next_adjust)) || ((gameMode[player] == 5) && (p_next_adjust)) )) {
						do {
							temp = Rand(7);
						} while((temp != 0) && (temp != 1) && (temp != 4) && (temp != 5));
					} else
						temp = Rand(7);

					if((same == 0) && (mae == temp)) {
						k = 1;
						same = limit[nextblock];
					}
					else k = 0;

				} while(((nextwork[temp] != 0) || (mae == temp)) && (k == 0));

				if(shu * i + j < 1400) nextb[i * shu + j + player * 1400] = temp;
				nextwork[temp] = 1;
				mae = temp;
			}
		}
	}

	nextc[player] = 0;
	next[player] = nextb[0 + player * 1400];

//	hole[player] = Rand(10);	//擬似乱数のためにブロックシャッターへ移動

	// NEXTブロックの色を設定 #1.60c7m9
	setNextBlockColors(player, 1);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  TGM風なNEXT生成処理#1.60c7h4
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// 変数playerを追加しました。#1.60c7m1
void tgmNextInit(int player) {
	int i, j;
	int history[4];
	int block;

//	for(i = 0; i < 4; i++) history[i] = 3; // 履歴を緑で埋める

	// Z S Z Sで埋める c7t3.1
		history[0] = 3;
		history[1] = 6;
		history[2] = 3;
		history[3] = 6;

	// 初手生成
	// next_adjustが動作してなかったのでとりあえず修正 #1.60c7i4
	if( ((gameMode[player] != 5) && (next_adjust)) || ((gameMode[player] == 5) && (p_next_adjust)) ) {
		do {
			nextb[0 + player * 1400] = Rand(7);
		} while((nextb[0 + player * 1400] == 2) || (nextb[0 + player * 1400] == 3) || (nextb[0 + player * 1400] == 6));
	} else {
		nextb[0 + player * 1400] = Rand(7);
	}
	// 初手生成時に履歴がずれていなかった LITE30.20より C7U1.5
	for(j = 0; j < 3; j++) {
		history[3 - j] = history[3 - (j + 1)];
	}

	history[0] = nextb[0 + player * 1400];

	// 残りのツモを生成
	for(i = 1; i < 1400; i++) {
		// ツモを引く
		block = Rand(7);

		// 引いたツモが履歴にあったら最大4回引き直し→6回に変更c7t3.1	これもリプレイには影響なし？
		if((block == history[0]) || (block == history[1]) || (block == history[2]) || (block == history[3])) {
			for(j = 0; j < 6; j++) {
				block = Rand(7);

				// 4つの履歴に無かったらその場で抜ける
				if((block != history[0]) && (block != history[1]) && (block != history[2]) && (block != history[3]))
					break;
			}
		}

		// 履歴をずらす
		for(j=0;j<3;j++) {
			history[3 - j] = history[3 - (j + 1)];
		}

		// 履歴に新しいブロックを入れる
		history[0] = block;

		// nextbに入れる
		nextb[i + player * 1400] = block;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ガイドライン対応ゲーム風なNEXT生成処理
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void guidelineNextInit(int player) {
	int i, j;
	int tmp;
	int first;
	int block[7];	// ブロックが出たかどうかのフラグ

	first = 1;
	//初手　ガイドライン風にもnextadjustを適用
	if( ((gameMode[player] != 5) && (next_adjust)) || ((gameMode[player] == 5) && (p_next_adjust)) ) {
		//初期化
		for(j = 0; j < 7; j++) block[j] = 0;

		do {
			tmp = Rand(7);
		} while((tmp == 2) || (tmp == 3) || (tmp == 6));

		// ブロックが出たフラグON
		block[tmp] = 1;

		// nextbに入れる
		nextb[0 + player * 1400] = tmp;
	} else {
		first = 0;	//次で初期化を行う
	}

	for(i = 0; i < 200; i++) {
		// 初期化	nextadjust適用時、最初は初期化しない
		if(!first)
			for(j = 0; j < 7; j++) block[j] = 0;

		// ツモ作成
		for(j = first; j < 7; j++) {
			do {
				tmp = Rand(7);	// ツモを引く
			} while(block[tmp] == 1);

			// ブロックが出たフラグON
			block[tmp] = 1;

			// nextbに入れる
			nextb[i * 7 + j + player * 1400] = tmp;
		}
		first = 0;	//フラグOFF
	}
}


// NEXT領域内のブロックの色を設定 #1.60c7m9
// 回転法則を変更したりNEXT関係の変数を弄ったりする度に呼び出す必要あり
void setNextBlockColors(int player, int first) {
	int i,j, tmp;

	if( first ) {
		// 初期化//ブロックの種類を決める
		c_nblk[0 + player * 6] = 1 + next[player];
		if( player == 0 ) {
			c_nblk[1 + player * 6] = 1 + nextb[(nextc[player] + 1) % 1400];
			c_nblk[2 + player * 6] = 1 + nextb[(nextc[player] + 2) % 1400];
			c_nblk[3 + player * 6] = 1 + nextb[(nextc[player] + 3) % 1400];
			c_nblk[4 + player * 6] = 1 + nextb[(nextc[player] + 4) % 1400];
			c_nblk[5 + player * 6] = 1 + nextb[(nextc[player] + 5) % 1400];
		} else {
			c_nblk[1 + player * 6] = 1 + nextb[( (nextc[player] + player * 1400) + 1 ) % 2800];
			c_nblk[2 + player * 6] = 1 + nextb[( (nextc[player] + player * 1400) + 2 ) % 2800];
			c_nblk[3 + player * 6] = 1 + nextb[( (nextc[player] + player * 1400) + 3 ) % 2800];
			c_nblk[4 + player * 6] = 1 + nextb[( (nextc[player] + player * 1400) + 4 ) % 2800];
			c_nblk[5 + player * 6] = 1 + nextb[( (nextc[player] + player * 1400) + 5 ) % 2800];
		}

		// 上で決まったブロック種類を元に色を変換する #1.60c7p1
		if( isWRule(player) ) {
			// ワールド
			for(i = 0; i < 6; i++) {
						c_nblk[i + player * 6] = wcol[ c_nblk[i + player * 6] - 1 ];
			}
		} else if( (rots[player] >= 4 ) && (rots[player] != 8)) {
			// ARS
			for(i = 0; i < 6; i++) {
						c_nblk[i + player * 6] = acol[ c_nblk[i + player * 6] - 1 ];
			}
		} else {
			// クラシック
			for(i = 0; i < 6; i++) {
						c_nblk[i + player * 6] = ccol[ c_nblk[i + player * 6] - 1 ];
			}

		}

		if(monochrome_mode[player]==1) {
			// モノクロモード
			for(i = 0; i < 6; i++) {
						c_nblk[i + player * 6] = 0;
			}
		} else if(((p_over1000)&&(gameMode[player] == 5))||(monochrome_mode[player] == 2)){	//即ブロックをを[]に
				if(rots[player] != 6){			//[]
					for(i = 0; i < 6; i++) {
						c_nblk[i + player * 6] = 8;
					}
				}else{
					if(heboGB[player] == 1){	//01
						for(i = 0; i < 6; i++) {
							c_nblk[i + player * 6] = 8;
						}
					}else{
						for(i = 0; i < 6; i++) {	//GB
							j=c_nblk[i + player * 6];
							c_nblk[i + player * 6] = c_nblk[i + player * 6]-(9*(j>=0));
						}
					}
				}
		}
	} else {
		// NEXT領域内のブロックの色をずらす
		c_nblk[0 + player * 6] = c_nblk[1 + player * 6];
		c_nblk[1 + player * 6] = c_nblk[2 + player * 6];
		c_nblk[2 + player * 6] = c_nblk[3 + player * 6];
		c_nblk[3 + player * 6] = c_nblk[4 + player * 6];
		c_nblk[4 + player * 6] = c_nblk[5 + player * 6];

		// NEXT領域の最後のブロックの色を設定
		if( player == 0 )
			c_nblk[5 + player * 6] = 1 + nextb[(nextc[player] + 5) % 1400];
		else
			c_nblk[5 + player * 6] = 1 + nextb[( (nextc[player] + player * 1400) + 5 ) % 2800];

		// 色を変換する #1.60c7p1
		if( isWRule(player) )
			c_nblk[5 + player * 6] = wcol[ c_nblk[5 + player * 6] - 1 ];
		else if( (rots[player] >= 4 ) && (rots[player] != 8))
			c_nblk[5 + player * 6] = acol[ c_nblk[5 + player * 6] - 1 ];
		else
			c_nblk[5 + player * 6] = ccol[ c_nblk[5 + player * 6] - 1 ];

		tmp = hnext[player] - 1;
		if(tmp < 0) tmp = 0;
		if(monochrome_mode[player] == 1)	//モノクロ
			c_nblk[5 + player * 6] = 0;
		// NEXT以降のブロックも[] #1.60c7s6
		if( (tc[player] >= over1000_start) ||(over1000_block[player])||((p_over1000)&&(gameMode[player] == 5))||(monochrome_mode[player] == 2) ) {
			if(rots[player] != 6){	//[]
				for(i = tmp; i < 6; i++) {
					c_nblk[i + player * 6] = 8;
				}
			}
			else{
				if(heboGB[player] == 1){	//01
						for(i = 0; i < 6; i++) {
							c_nblk[i + player * 6] = 8;
						}
				}
				for(i = tmp; i < 6; i++) {	//GB
					j=c_nblk[i + player * 6];
					c_nblk[i + player * 6] = c_nblk[i + player * 6]-(9*(j>=0));
				}
			}
		}
		//回転封じ中 ブロックの向きをずらす
		if(isrotatelock[player]){
			rt[player] = rt_nblk[0 + 6 * player];
			rt_nblk[0 + 6 * player] = rt_nblk[1 + 6 * player];
			rt_nblk[1 + 6 * player] = rt_nblk[2 + 6 * player];
			rt_nblk[2 + 6 * player] = rt_nblk[3 + 6 * player];
			rt_nblk[3 + 6 * player] = rt_nblk[4 + 6 * player];
			rt_nblk[4 + 6 * player] = rt_nblk[5 + 6 * player];
			rt_nblk[5 + 6 * player] = rand(4,player);
		} else if(ace_irs){
			rt[player] = rt_nblk[0 + 6 * player];
			rt_nblk[0 + 6 * player] = 0;
		}

		// アイテムをずらす
		item[player] = item_nblk[0 + player * 6];
		item_nblk[0 + player * 6] = item_nblk[1 + player * 6];
		item_nblk[1 + player * 6] = item_nblk[2 + player * 6];
		item_nblk[2 + player * 6] = item_nblk[3 + player * 6];
		item_nblk[3 + player * 6] = item_nblk[4 + player * 6];
		item_nblk[4 + player * 6] = item_nblk[5 + player * 6];
		item_nblk[5 + player * 6] = 0;

		if((item_mode[player]) || ((gameMode[player] == 0) && (novice_mode[player])))
			item_nblk[2 + player * 6] = item_coming[player];
		else
			item_nblk[(hnext[player]-1) + player * 6] = item_coming[player];
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  プレイヤーのステータスに応じて各処理を実行
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void playerExecute(void) {
	int		i, newhole, j, k, temp,pages2;
//	グローバル変数playerは廃止しました。
//	代わりに「i」を用いてください。

	if(flag == 1) return;

	// 捨てゲー
	if(doGiveup()) {
		return;
	}

	// PRACTICEでの死亡処理
	if(flag == -1) {
		PracticeDeath();
		return;
	}
	// 対戦での死亡処理
	if(flag == -2) {
		winner();
		return;
	}

	if(flag == -3){
		winner2();
		return;
	}

	// 窒息しそうなら警告音を鳴らす #1.60c7l5
	// プレイ中のみ #1.60c7l6
	if( ((pinch[0])&&(onRecord[0])) || ((pinch[1])&&(onRecord[1])) ) {
		if( !IsPlayWave(40) ) PlaySE(40);
	} else {
		// ピンチから通常状態に戻ったら即止める #1.60c7l6
		StopSE(40);
	}


	// ステージスキップキー長押し
	if( IsPressKey(skipKey) ){
			stage_skip_mpc[0]++;
			}else{
			stage_skip_mpc[0]=0;
	}
	// 長押しされたら実行
	if( (gameMode[0] == 6) && (timeOn[0]) && (ltime[0] > 1800) && (stage[0] < 19 )
	 && (stage_skip_mpc[0] >= 60)) {
		replay_save[0] = 0;				// リプレイ記録不可 #1.60c7n8
		ltime[0] = ltime[0] - 1800;		// -30秒
		timeOn[0] = 0;					// タイマーストップ
		sclear[0] = 0;					// スキップ
		recFaultTime(0);				//ステージクリアしていない
		statc[0 * 10] = 0;				// カウンタを0に
		statc[0 * 10 + 1] = 0;
		stat[0] = 17;					// ステージ終了
		stage_skip_mpc[0] = 0;
	}

	for(i = 0; i < 1 +maxPlay; i++) {

		if(pause[i]) {
			if( (count % 40 < 20) && (!debug) ){
				printFont(17 + 24 * i - 12 * maxPlay, 15, "PAUSE!", fontc[rots[i]]);
			}
			if(gameMode[i] == 5){
				if(stat[i] == 5){
					drawCBlock (i, blk[i] + 1, 0, 0 , 0 , 0);
					if(spawn_y_type)// フィールド枠
						viewFldFrame(1,i);
				}
				if((item_mode[i])||(novice_mode[i])){
					printFont(14 - 12 * maxPlay, 16, "C+<L R>:ITEM", count % 9);
					if(getPressState(i, 6)){
						if(getPushState(i, 2)){
							PlaySE(5);
							item_nblk[0 + i * 6]--;
							if(item_nblk[0 + i * 6] < 0) item_nblk[0 + i * 6] = item_num;
							item_name[i] = item_nblk[0 + i * 6];
						}
						if(getPushState(i, 3)){
							PlaySE(5);
							item_nblk[0 + i * 6]++;
							if(item_nblk[0 + i * 6] > item_num) item_nblk[0 + i * 6] = 0;
							item_name[i] = item_nblk[0 + i * 6];
						}
					}
				}
			}
			goto skip;
		}

		// X-RAY用カウント#1.60c7p9ex
		if ( isxray[i] ) {
			xray_counter[i] ++;
		}

		// COLOR用カウント#1.60c7p9ex
		if ( iscolor[i] ) {
			color_counter[i] ++;
		}

		// 花火用カウント #1.60c7q5
		if( hanabi_waiting[i] ) {
			hanabi_frame[i] = hanabi_frame[i] + 3;
			if(hanabi_waiting[i] > 60)
				hanabi_frame[i] = hanabi_frame[i] + 3;

			if(hanabi_frame[i] >= 30) {
				hanabi_waiting[i]--;
				PlaySE(35);
				objectCreate2(i, 7, Rand(80) + 72 + 192 * i - 96 * maxPlay, 16 + Rand(20) + 116 * ((checkFieldTop(i) < 12) && (by[i] < 12)), 0, 0, Rand(7)+1, 0);
				//hanabi_total[i]++;//総数カウント
				hanabi_frame[i] = 0;
			}
		}else{
			hanabi_frame[i] = 30;
		}
		if((ending[i] == 2) && (gameMode[i] == 0)&&(endtime[i] % hanabi_int[i] == 0)&&(!novice_mode[i])){//ロール中花火　staffroll.cから移動
			hanabi_total[i]++;
			PlaySE(35);
			objectCreate2(i, 7, Rand(80) + 72 + 192 * i - 96 * maxPlay, 16 + Rand(20) + 116 * ((checkFieldTop(i) < 12) && (by[i] < 12)), 0, 0, Rand(7)+1, 0);
		}


		if(item_timer[i]) item_timer[i]--;


		//プラチナブロック消去サウンド
		if(platinaerase_cnt[i] != 0){
			if(platinaerase_cnt[i] % 10 == 0){
					PlaySE(41);
			}
		platinaerase_cnt[i]--;
		}
		//対戦での時間制アイテムの効果終了
		if( (gameMode[i] == 4) || (item_mode[i]) ) {
			if( (isrotatelock[i]) && (item_timer[i] <= 0) ) {
				// ROTATE LOCK
				isrotatelock[i] = 0;
				// NEXTブロックの向きを元に戻す
				rt_nblk[0 + 6 * i] = 0;
				rt_nblk[1 + 6 * i] = 0;
				rt_nblk[2 + 6 * i] = 0;
				rt_nblk[3 + 6 * i] = 0;
				rt_nblk[4 + 6 * i] = 0;
				rt_nblk[5 + 6 * i] = 0;
				item_timer[i] = 0;
				if(cpu_flag[i]) {
					cpuCheckBestSpot(i);
				}
			} else if( (ishidenext[i]) && (item_timer[i] <= 0) ) {
				// HIDE NEXT
				ishidenext[i] = 0;
				item_timer[i] = 0;
			} else if( (isLRreverse[i]) && (item_timer[i] <= 0) ) {
				// LR reverse
				isLRreverse[i] = 0;
				item_timer[i] = 0;
			} else if( istimestop[i] && (item_timer[i] <= 0)) {
				// 時間停止
				istimestop[i] = 0;
				item_timer[i] = 0;
				StopSE(47);
			} else if( (isboost[i]) && (item_timer[i] <= 0) ) {
				// BOOST FIRE
				isboost[i] = 0;
				item_timer[i] = 0;
			} else if( (isfever[i]) && (item_timer[i] <= 0)) {
				// フィーバー
				if(!isfever[1-i])
					StopWave(65);
				if((!IsPlayWave(50+bgmlv)) && (!isfever[1-i]) && (!ending[i]) && (timeOn[i]))
					PlayWave(50 +bgmlv);
				isfever[i] = 0;
				item_timer[i] = 0;
			} else if( (isUDreverse[i]) && (item_timer[i] <= 0) ) {
				// UD reverse
				isUDreverse[i] = 0;
				item_timer[i] = 0;
			} else if( (isreflect[i]) && (item_timer[i] <= 0) ) {
				// reflect
				isreflect[i] = 0;
				item_timer[i] = 0;
			}  else if( (isdark[i]) && (item_timer[i] <= 0) ) {
				// ダーク
				isdark[i] = 0;
				item_timer[i] = 0;
			} else if( (isdouble[i]) && (item_timer[i] <= 0) ) {
				// ダブル
				isdouble[i] = 0;
				item_timer[i] = 0;
			} else if( (isfakenext[i]) && (item_timer[i] <= 0) ) {
				// fake NEXT
				isfakenext[i] = 0;
				item_timer[i] = 0;
			}
		}

		// 15分で超高速#1.60c7h3
		// 横溜めは高速化させない #1.60c7k8
		// speed.defで速度を変えられる #1.60c7l2
		// ミッションでは無効 C7T6.6
		if((gameMode[i] <= 4) || (gameMode[i] == 10)) {
			if((time[i] > 54000) && (limit15)) {
				sp[i]    = speed_limit15;
				wait1[i] = wait1_limit15;
				wait2[i] = wait2_limit15;
				wait3[i] = wait3_limit15;
			}
		}

		if((istimestop[i]) && (repversw >= 35)){
			if(stat[i] == 5){
				drawCBlock (i, blk[i] + 1, 3, 0 , 0 , 0);
				if(spawn_y_type)// フィールド枠
					viewFldFrame(1,i);
			}
			goto next;
		}
		// jump(stat[i],l00,l01,l02,l03,l04,l05,l06,l07,l08,l09,l10,l11,l12,l13,l14,l15,l16,l17,l18,l19,l20,l21,l22,l23,l24,l25,l26,l27,l28,l29,l30,l31,l32,l33,l34,l35,l36,l37,l38);
//sprintf(string[0],"%2d STAT",stat[i]);
//printFont(0, 3, string[0], (count % 4 / 2) * digitc[rots[i]]);
		switch ( stat[i] ) {
			case 0: goto l00;
			case 1: goto l01;
			case 2: goto l02;
			case 3: goto l03;
			case 4: goto l04;
			case 5: goto l05;
			case 6: goto l06;
			case 7: goto l07;
			case 8: goto l08;
			case 9: goto l09;
			case 10: goto l10;
			case 11: goto l11;
			case 12: goto l12;
			case 13: goto l13;
			case 14: goto l14;
			case 15: goto l15;
			case 16: goto l16;
			case 17: goto l17;
			case 18: goto l18;
			case 19: goto l19;
			case 20: goto l20;
			case 21: goto l21;
			case 22: goto l22;
			case 23: goto l23;
			case 24: goto l24;
			case 25: goto l25;
			case 26: goto l26;
			case 27: goto l27;
			case 28: goto l28;
			case 29: goto l29;
			case 30: goto l30;
			case 31: goto l31;
			case 32: goto l32;
			case 33: goto l33;
			case 34: goto l34;
			case 35: goto l35;
			case 36: goto l36;
			case 37: goto l37;
			case 38: goto l38;
		}

		l00: statJoinwait(i);			// 00 -> 途中参加待ち状態
				goto next;
		l01: statBlockSutter(i);		// 01 -> ブロックシャッター
				goto next;
		l02: statSelectMode(i);			// 02 -> モードセレクト画面
				goto next;
		l03: statReady(i);				// 03 -> Ready!?
				goto next;
		l04: statBlock(i);				// 04 -> ブロック落下準備
				goto next;
		l05: statMove(i);				// 05 -> ブロック移動中
				goto next;
		l06: statErase(i);				// 06 -> 時間稼ぎ
				goto next;
		l07: statGameOver(i);			// 07 -> ゲームオーバー演出
				goto next;
		l08: statEraseBlock(i);			// 08 -> ブロック消去
				goto next;
		l09: statSelectLevel(i);		// 09 -> レベルセレクト画面
				goto next;
		l10: statVersusWait(i);			// 10 -> おまちください画面
				goto next;
		l11: statNameEntry(i);			// 11 -> ネームエントリー
				goto next;
		l12: statSelectStartLv(i);		// 12 -> debug = 1のときのレベルセレクト
				goto next;
		l13: statEnding(i);				// 13 -> エンディング演出
				goto next;
		l14: statResult(i);				// 14 -> 結果画面
				goto next;
		l15: statRelayselect(i);		// 15 -> ROT.relayモードセレクト画面　
				goto next;					// どうみてもSHIRASE風せり上がりは使用されていないので再利用
		l16: statVersusSelect(i);		// 16 -> 対戦モードセレクト画面
				goto next;
		l17: statTomoyoNextStage(i);	// 17 -> TOMOYOモード ステージクリア／タイムオーバー (tomoyo.c)
				goto next;
		l18: statTomoyoEditor(i);		// 18 -> TOMOYOモード ステージエディタ (tomoyo.c)
				goto next;
		l19: statTomoyoSelect(i);		// 19 -> TOMOYOモード セレクト画面 (tomoyo.c)
				goto next;
		l20: statTomoyoResult(i);		// 20 -> TOMOYOモード 結果表示 (tomoyo.c)
				goto next;
		l21: statGameOver2(i);			// 21 -> ゲームオーバー表示
				goto next;
		l22: statNothing(i);			// 22 -> 一定時間何もしない
				goto next;
		l23: statMissionSelect(i);		// 23 -> MISSIONモード セレクト画面
				goto next;
		l24: statMissionEditor(i);		// 24 -> MISSIONモード エディット画面
				goto next;
		l25: statDelField(i);			// 25 -> ライン強制消去
				goto next;
		l26: statLaser(i);				// 26 -> レーザー
				goto next;
		l27: statNegafield(i);			// 27 -> ネガフィールド
				goto next;
		l28: statShotgun(i);			// 28 -> ショットガン
				goto next;
		l29: statExchangefield(i);		// 29 -> フィールド交換
				goto next;
		l30: statExamination(i);		// 30 -> 段位試験っぽいの
				goto next;
		l31: statItemRulet(i);			// 31 -> アイテムルーレット
				goto next;
		l32: statFreefall(i);			// 32 -> 自由落下
				goto next;
		l33: statMovfield(i);			// 33 -> ブロックの壁寄せ
				goto next;
		l34: stat180field(i);			// 34 -> ブロックをひっくり返す
				goto next;
		l35: statDelfromUpper(i);		// 35 -> 上からライン強制消去
				goto next;
		l36: statTomoyoSelect_FP(i);	// 36 -> TOMOYO F-Pointモード セレクト画面 (tomoyo.c)
				goto next;
		l37: statSelectStandardSp(i);	// 37 -> STANDARD速度ロード
				goto next;
		l38: statReplaySave(i);			// 38 -> リプレイ保存メニュー
				goto next;
		next:

		if(isbanana[i])
			statBanana(i);				// EX -> SHUFFLE FIELD

		// タイム増加
		increment_time(i);

		skip:	;
	}


	// T-SPIN 残り表示フレーム数を減少 #1.60c7n6
	if(tspin_c[0]) tspin_c[0]--;
	if(tspin_c[1]) tspin_c[1]--;

	if(b_to_b_c[0]) b_to_b_c[0]--;
	if(b_to_b_c[1]) b_to_b_c[1]--;


	// BGMフェードアウト
	bgmFadeout();



	// 両者ゲームオーバーなら一定時間経過後タイトルへ
	if((stat[0] == 0) && (stat[1] == 0)) {
		if(!overcount) {
			versusInit(0);
			setNextBlockColors(0,1);	// #1.60c7m9
			hold[0] = -1;

			if(maxPlay) {
				versusInit(1);
				setNextBlockColors(1,1);
				hold[1] = -1;
			}
		}
		if(playback) {
			restoreSetups();
			if(!tmp_maxPlay)
				maxPlay = 0;
			if(gameMode[0] == 8) gameMode[0] = 0;
			flag = 1;
		}
		overcount++;
		if(overcount > 300) {
			if( !playback ){
				if(ranking_type==0){
					RankingProc_1(gameMode[0]);
				}else if(ranking_type==1){
					RankingProc_2(gameMode[0]);
				}else{
					if((enable_grade[0]==3)||(enable_grade[0]==4)||(anothermode[0]>=2)||(tomoyo_opt[0]>=4)||(std_opt[0]>=2)){
					pages2=1;//各モードの2画面目
					}else{
					pages2=0;
					}
					RankingProc_3(gameMode[0],pages2);
				}
			flag = 1;
			setupBak[4] = rots[0] +rots[1] * 10;	// #1.60c5
			}
		}
	} else {
		overcount = 0;
	}
}

/* 捨てゲーする(した場合の戻り値は1、しなかったら0) */
// playerExecuteから独立#1.60c7j6
int doGiveup() {
	int i, tmp1, tmp2;

	// giveup_safety修正 #1.60c7p4
	if(giveup_safety) {
		if(!pause[0] && !pause[1]) return 0;
	}

	// 捨てゲーキーをjoypadに割り当て 1.60c7g7
	if(getPressState(0, 8)) mpc3[0]++;
	else mpc3[0] = 0;

	if(getPressState(1, 8)) mpc3[1]++;
	else mpc3[1] = 0;

	// いつでも捨てゲーできるようにした#1.60cd
	// リプレイが再生できないので修正#1.60c7h1
	// デモ画面では捨てゲーできないように変更 #1.60c7o8
	if((!playback) && (!demo) && (IsPushKey (giveupKey) || (mpc3[0] == 1) || (mpc3[1] == 1))) {// Qキー捨てゲー #1.60c
		// 効果音停止
		StopAllWaves();
		StopAllBGM();

		// 背景を戻す #1.60c7o8
		bgfadesw = 0;
		if(gameMode[0] != 5) backno = 0;

		pause[0] = 0; // ポーズ解除
		pause[1] = 0;
		timeOn[0] = 0;
		timeOn[1] = 0;
		onRecord[0] = 0;
		onRecord[1] = 0;
		cpu_flag[0] = 0;
		cpu_flag[1] = 0;


		// スタッフロール消去 #1.60c7n5
//		staffInit2();
		objectClear();
		if((gameMode[0] == 6) && (tmp_maxPlay != maxPlay))
			maxPlay = tmp_maxPlay;
		if(gameMode[0] == 8) {
			// MISSION
			enterMissionMode(0);
			return 1;
		} else if(gameMode[0] == 5) {
			// PRACTICE
			flag = -1;
			return 0;
		} else if(gameMode[0] == 4) {
			// 対戦モード
			enterVersusMode(0);
			flag = 0;

			// WAIT値をバックアップから戻す #1.60c7p4
			for(i=0; i<2; i++) {
				wait1[i] = b_wait1[i];
				wait2[i] = b_wait2[i];
				wait3[i] = b_wait3[i];
				waitt[i] = b_waitt[i];
				sp[i]    = b_sp[i];
			}

			return 1;
		} else {
			// 通常モード
			randommode[0] = 1;
			randommode[1] = 1;
			versusInit(0);
			versusInit(1);
			randommode[0] = 0;
			randommode[1] = 0;

			tmp1 = stat[0];
			tmp2 = stat[1];

			if(examination[0]==2){
				admit_grade[0]=exam_grade[0];//降格試験中だったら強制的に落とす
				PlayerdataSave();//セーブ
			}

			playerInitial(0);
			playerInitial(1);


			// 捨てゲーした時の動作を選べるようにした#1.60c7i6
			if(giveup_func == 1) {
				if(tmp1 != 0){
					stat[0] = 2;
					statc[0 * 10 + 4] = 16;
				}
				if(tmp2 != 0){
					stat[1] = 2;
					statc[1 * 10 + 4] = 16;
				}
			} else if(giveup_func == 2) {
				flag = 1;
			}

			return 1;
		}
	}
	return 0;
}

/* 経過タイムを増加 */
void increment_time(int player) {
	int		i, j, k, temp;

	// ロールクリア
	if((ending[player] == 2) && (edrec[player] > 3740) && (onRecord[player])) {
		// スタッフロール表示を消す #1.60c7n5
		staffInitPl(player);
		if((gameMode[player] >= 1) && (gameMode[player] <= 2)){
		if(gameMode[player] == 6)
			tomoyo_ehfinal_c[player] = 0;

		// ロールクリアでGMになる #1.60c7i2
		if(enable_grade[player] == 1){//1
			if(gmflag_r[player]) {
				grade[player]=32;
				gflash[player]=120;
				PlaySE(30);	// rankup.wav
			}
		}else if(enable_grade[player]== 2){
			if((gmflag_r[player])&&(endlines[player] >= 20)) {//消えロール20列消
				grade[player]=grade[player] + 3;	//合計+3
				gflash[player]=120;
				PlaySE(30);	// rankup.wav

			}else if((gmflag_r[player])&&(endlines[player] >= 13)) {//13列消
				grade[player]=grade[player] + 2;	//合計+2
				gflash[player]=120;
				PlaySE(30);	// rankup.wav

			}else if((gmflag_r[player])&&(endlines[player] >= 7)) {//7列消
				grade[player]=grade[player] + 1;	//合計+1
				gflash[player]=120;
				PlaySE(30);	// rankup.wav
			}else if((gmflag_r[player])&&(endlines[player] < 3)){//3列しか消してない
				grade[player]=grade[player] - 1;
			}else if(!gmflag_r[player]){//見えロール
				grade[player]=grade[player] + 1;	//合計+1
				gflash[player]=120;
				PlaySE(30);	// rankup.wav
			}
		}else if(enable_grade[player] == 3){
			if((gmflag_r[player])&&(endlines[player] >= 15)) {
				grade[player]=32;
				gflash[player]=120;
				PlaySE(30);	// rankup.wav
			}
		}else if(enable_grade[player] == 4){
			if((gmflag_r[player])&&(endlines[player] >= 3)) {//消えロール
				grade[player] = grade[player] + endlines[player] / 4;
				if((grade[player]>= 30)&& (tr[player] >175)&&(viewgrade(player)>=31)){//MMを取ってないとだめ
					grade[player] = 32;//GM
					PlaySE(30);	// rankup.wav
				}else if((grade[player]>= 30)&& (tr[player] >155)){
					grade[player] = 31;//MM
					PlaySE(30);	// rankup.wav
				}else if(grade[player] >= 30){
					grade[player] = 29;//MV
					PlaySE(30);	// rankup.wav
				}
				if(grade[player]>32)grade[player]=32;
			}
			if((gmflag_r[player])&&(endlines[player] < 3)){//3列も消してない
				grade[player]=grade[player] - 1;
			}
			if(!gmflag_r[player]){//見えロール
				grade[player]= grade[player] + endlines[player] / 10;
				if(grade[player] >= 28){
					grade[player] = 28;
				}
				if(grade[player]>32)grade[player]=32;
			}
		}
		}
		if(gameMode[player] == 3) {
			if((devil_minus[player])&&(gameMode[player] == 3)&&(repversw >= 31) && (tc[player] >= 1000)){//devil-
				grade[player] = 15;
				gflash[player]=120;
			}else if(!devil_minus[player]){
				// CLASSIC系列は5:00以内、WORLD系列は5:20以内でGOD昇格
				if( ((!isWRule(player)) && (time[player] <= 18000)) || ((isWRule(player)) && (time[player] <= 19200)) ) {
					grade[player] = 16;
					PlaySE(30);	// rankup.wav
					gflash[player]=120;
				}
				else{  //そうでなければS13止まり
					grade[player] = 13;
					gflash[player]=120;
				}
			}
		}

		stat[player] = 13;
		for(i=0; i<10; i++) statc[player * 10 + i] = 0;
		if(item_mode[player]){
			statusClear(player);
			misstimer[player] = 0;
			item_t[player] = 0;
			item_timer[player] = 0;
		}
		ending[player] = 3;
		if((devil_minus[player])&&(gameMode[player] == 3) && (tc[player] == 500)){
			end_f[player] = 0;//devil-ラインなし
			ending[player] = 5;//500足切り
		}else if(gameMode[player] >= 1){
			end_f[player] = 2;//オレンジライン
		}
		if(gameMode[player] == 5){
			ending[player] = 7;	// PRACTICEは超短縮
			statc[player * 10] = 133;
		}

		onRecord[player] = 0;					// エンディングロール終了と同時にリプレイ記録終了
		if(gameMode[player] == 8)
			PlaySE(36);
		else if(ending[player] == 3)
			PlaySE(29);

		// フィールドタイマー解除 #1.60c7k7
		disableShadowTimer(player);

		// 念のためX-RAY・COLORをなくす#1.60c7p9ex
		isxray[player] = 0;
		iscolor[player] = 0;
		istimestop[player] = 0;
		item_timer[player] = 0;
	}

	// リプレイ用データを記録
	if(onRecord[player] != 0) {
		if( (!playback) && (!demo) && (replay_save[player]) && (time2[player] < 60 * 60 * 20) && (!fpbas_mode[player])) {
			temp = 0;
			for(j = 0; j < 8; j++)
				temp = temp | (getPressState(player, j) << j) | (getPushState(player, j) << (j + 8));

			replayData[time2[player] + player * 60 * 60 * 20] = temp;
		}

		time2[player]++;	// リプレイ用プレイ時間に1/60秒プラス #1.60c7n8
	}

	// タイマーが動作していれば
	if(timeOn[player]) {
		time[player]++;		// 時間に1/60秒プラス
		if((gameMode[player] == 6) && (stage[player] == 44))
			tomoyo_ehfinal_c[player]++;
			if(tomoyo_ehfinal_c[player] > 480)
				tomoyo_ehfinal_c[player] = 0;
	}

	// エンディング中ならばエンディング経過時間1/60秒プラス
	if(( ((ending[player] > 0) && (repversw < 33)) || ((ending[player] == 2) && (repversw >= 33))) && (onRecord[player])) {
		if((repversw >= 50)&&(fastroll[player])){
			edrec[player]= edrec[player]+2;//経過時間が2倍速
		}else{
			edrec[player]++;
		}
		//無限回転対策（BEGINNER以外）
		if((repversw >= 55) && (gameMode[player] >= 1) && ((kickc[player] > 10) || (kickc3[player] > 8)) ){
			edrec[player] = edrec[player] - 1 - (fastroll[player]);
			printFont(13 + 24 * player - 12 * maxPlay, 25, "PUT THE BLOCK!", (count % 4 / 2) * 2);
		}
	}
	if((gameMode[player] == 6) && (stage[player] == 44) && (ending[player] == 2)){
		tomoyo_ehfinal_c[player]++;
		if(tomoyo_ehfinal_c[player] > 480)
			tomoyo_ehfinal_c[player] = 0;
	}

	//レベルストップ中のタイム
	if((timeOn[player]) && (gameMode[player]<= 2)
	&&((tc[player] % 100 == 99)||(tc[player] == 998))) {
		time99[player]++;		// gradeup3で使う
	}

	//デビルでのレベルストップ中のタイム
	if((timeOn[player]) && (gameMode[player] == 3)&&(tc[player] % 100 == 99)) {
		time99[player]++;		// gradeup3で使う
	}

	// VSモードの制限時間をカウントダウン #1.60c7r5
	if( (timeOn[player]) && (gameMode[player] == 4) && (vs_time != 0) ) {
		ltime[player]--;

		// 10秒前の警告音 #1.60c7r9
		if(ltime[player] == 10 * 60) PlaySE(32);

		// タイムオーバー
		if(ltime[player] <= 0) {
			timeOn[0] = 0;
			timeOn[1] = 0;
			onRecord[0] = 0;
			onRecord[1] = 0;
			ltime[0] = 0;
			ltime[1] = 0;
			PlaySE(33);//タイムオーバー音
			if(wintype == 1){
				if(li[0] == li[1]) {
					// レベルが同じなら引き分け
					stat[0] = 7;
					stat[1] = 7;
				} else if(li[0] > li[1]) {
					// 1Pのレベルが2Pより高いなら1Pの勝ち
					stat[1] = 7;
				} else {
					// 2Pのレベルが1Pより高いなら2Pの勝ち
					stat[0] = 7;
				}
			}else{
				if(tc[0] == tc[1]) {
					// レベルが同じなら引き分け
					stat[0] = 7;
					stat[1] = 7;
				} else if(tc[0] > tc[1]) {
					// 1Pのレベルが2Pより高いなら1Pの勝ち
					stat[1] = 7;
				} else {
					// 2Pのレベルが1Pより高いなら2Pの勝ち
					stat[0] = 7;
				}
			}

			for(i=0; i<20; i++) statc[i] = 0;
		}
	}

	// TOMOYOモードの制限時間をカウントダウン #1.60c7l5
	if( (timeOn[player]) && (gameMode[player] == 6) ) {
		ctime[player]++;	//プレイ時間増加
		if(!fpbas_mode[player]){
			ltime[player]--;	//リミットタイム
		}
		if((FP_bonus[player] > 0) && (stage[player] >= 100)){
			if(repversw < 47) FP_bonus[player]--;
			else{
				if((stat[player] != 6) && (stat[player] != 8))
					FP_bonus_c[player]--;
				if(FP_bonus_c[player] <= 0){
					FP_bonus_c[player] = 4;
					FP_bonus[player]--;
				}
			}
		}

		// 0になったらゲームオーバー
		if(ltime[player] <= 0) {
			setGameOver(player);
		}
		// まだリミットタイムが0になっていない
		// ステージ21以降はステージタイム無限 #1.60c7m1
		else if(stage[player] <= 19)
		{
			if(stime[player] > 0) {
				stime[player]--;	//ステージタイム
				if(stime[player] == 10 * 60) PlaySE(32);
				if(stime[player] == 0) PlaySE(33);
			}
		}
	}

	// ACEモードの制限時間をカウントダウン
	if( (timeOn[player]) && (ltime[player] > 0) &&( (gameMode[player] == 8) || ((gameMode[player] == 7)&&(anothermode[player]!=3)) ) ) {
		ltime[player]--;

		//耐久ミッションでの無限回転対策
		if((gameMode[player] == 8) && (mission_type[c_mission] == 23) && ((kickc[player] > 10) || ((kickc3[player] > 8-(4*(repversw>=43))) && (repversw >= 40))) ){
			ltime[player]++;
			printFont(13 + 24 * player - 12 * maxPlay, 29, "PUT THE BLOCK!", (count % 4 / 2) * 2);
		}

		// 10秒前の警告音 #1.60c7r9
		if(ltime[player] == 10 * 60) PlaySE(32);

		// 0になったらゲームオーバー（ただし耐久ミッションは除く）
		if(ltime[player] <= 0) {
			if(gameMode[player] == 7){	//ACE
				PlaySE(33);
				setGameOver(player);
			} else if((gameMode[player] == 8) && (mission_type[c_mission] != 23)){	//MISSION
				PlaySE(33);
				setGameOver(player);
			}
		}
	}		//ULTRA
	if((timeOn[player])&& (ltime[player] > 0)&&(gameMode[player] == 9)&&(std_opt[player] == 1)) {
		if(ltime[player] == 10 * 60) PlaySE(32);
		ltime[player]--;

		if(ltime[player] <= 0){
			checkEnding(player, 0);
		}
	}
	//PRAC
	if((timeOn[player])&& (ltime[player] > 0)&&(gameMode[player] == 5)&&(p_goaltype == 4)) {
		if(ltime[player] == 10 * 60) PlaySE(32);
		ltime[player]--;

		if(ltime[player] <= 0){
			checkEnding(player, 0);
		}
	}

	// シャドウタイマーをカウントダウン #1.60c7j5
	if((hidden[player] == 8) && (m_roll_blockframe)){
		for(j = 0;j < fldsizew[player] * (fldsizeh[player] + 1); j++)
			if(fldt[j + player * 220] < 0) fldt[j + player * 220] = fldt[j + player * 220] + 10;
	}else if(shadow_timer_flag[player]){
		for(j = 0;j < fldsizew[player] * (fldsizeh[player] + 1); j++){
			if(hidden[player]==9){
				if(fldt[j + player * 220] > 0) fldt[j + player * 220]=fldt[j + player * 220]-3;
			}else if(hidden[player]==10){
				if(fldt[j + player * 220] > 0) fldt[j + player * 220]=fldt[j + player * 220]-2;
			}else{
				if(fldt[j + player * 220] > 0) fldt[j + player * 220]=fldt[j + player * 220]-1;
			}
		}

	}
	// 段位ポイント減少 #1.60C7T5EX
	if(((enable_grade[player]== 2)||(enable_grade[player]== 3))&&((gameMode[player] == 1) || (gameMode[player] == 2)) ) {
		if( ((stat[player] == 3) || (stat[player] == 5)) && (combo[player] <= 1) && (!ending[player]) ) {
			gtime[player]++;

			if(((grade[player] > 0)||(gpoint[player] > 0))&& (gtime[player] >= glimit[grade[player]]) ) {
				gpoint[player]--;
				gtime[player] = 0;
				if(enable_grade[player] == 2){
					if(gpoint[player] < -50) {
						grade[player]--;
						gpoint[player] = 0;
						//gflash[player] = 120;
						gup_down[player] = 0;//regret
						PlaySE(45);
					}
				}else if(enable_grade[player] == 3){
					if(gpoint[player] < 0)
						gpoint[player] = 0;
				}
			}
		}
	}
	// 段位上昇アニメ時間減少
	if( (gflash[player] > 0) && ((!tspin_c[player]) || (enable_grade[player] != 4) || (gameMode[player] >= 3)) ) {
		gflash[player]--;
	}
	if((gflash[player] > 0)&&(gameMode[player] == 0)&&(novice_mode[player])&&(time[player] < 18000)){
			printFont(18 + 24 * player - 12 * maxPlay, 11, "TIME", (count % 4 / 2) * digitc[rots[player]]);
			printFont(17 + 24 * player - 12 * maxPlay, 12, "BONUS!", (count % 4 / 2) * digitc[rots[player]]);
			sprintf(string[0],"%6d PTS",1253 * (300 - (time[player] / 60)));
			printFont(15 + 24 * player - 12 * maxPlay, 14, string[0], (count % 4 / 2) * digitc[rots[player]]);
	}
	if(hnext_flag[player]){
		hnext_timer[player]++;
		if(hnext_timer[player] > 720) hnext_timer[player] = 0;
		if(((hnext_timer[player] > 540) && (hnext_timer[player] <= 660)) || (( ((hnext_timer[player] > 480) && (hnext_timer[player] <= 540)) || (hnext_timer[player] > 660) ) && (hnext_timer[player] % 2 == 0)))
			ishidenext[player] = 1;
		else
			ishidenext[player] = 0;
	}
	if(onRecord[player] == 2){//2なら現フレームのリプレイデータを記録後、記録を終了する
		time2[player]++;
		onRecord[player] = 0;
	}
}

// 現在のルールがワールド系かどうか判定 #1.60c7r3
int isWRule(int player) {
	return ( (rots[player] == 2) || (rots[player] == 3) || (rots[player] == 6) ||(rots[player] == 7) );
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.00 - 参加待ち中状態
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statJoinwait(int player) {
	padRepeat(player);

	if(count % 80 > 10) {
		if(!playback) {
			ExBltRect(81, 110+192 * player -96 * maxPlay , 85 , 0 + 100 * player, 50, 100, 65);
		}
	}

	// ボタンが利きにくいのを修正#1.60cf
	if((getPushState(player, 4)) && (tomoyo_domirror[1-player] == 0) && !((player == 1) && ((domirror) && (mirror)) ) && (!bgfadesw)) {
		StopSE(8);
		PlaySE(10);

		if(wavebgm >= 1) {
			if(!IsPlayWave(62)) PlayWave(62);
		}
		playerInitial(player);
		randommode[player] = 1;
		versusInit(player);
		randommode[player] = 0;
		first_rot[player] = rots[player];

		hold[player] = -1;					// holdリセット #1.60c
		stat[player] = 1;					// ブロックシャッター実行
		statc[player * 10] = 0;				// ステータスカウンタを0に
		statc[player * 10 + 1] = 2;			// シャッター後はステータスNo.2
		statc[player * 10 + 2] = 0;			// シャッター後はステータスNo.2
		statc[player * 10 + 4] = 16;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.01 - ブロックシャッター
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statBlockSutter(int player) {
	// すぐに次のステータスへ行けるように#1.60cd
	int		i, j, blk;

	StopAllBGM();
	if(!playback){
		GiziSRand(player);				// 擬似乱数のシードを生成（リプレイ時除く）
		max_hnext[player] = hnext[player];
		first_rot[player] = rots[player];
	}
	if((gameMode[player] == 4) || (gameMode[player] == 8))
		hole[player] = rand(10,player);	// DS-RANDOMせり上がりのためにここで穴の位置を設定

	stat[player] = statc[player * 10 + 1];	// 次のステータスへジャンプ
	statc[player * 10    ] = 0;				// カウンタはあとかたづけ
	statc[player * 10 + 1] = 0;

	for(i=0;i<10;i++) {	//cpu入力初期化
		cp_input[i + player * 10] = 0;
	}

	if(gameMode[player] != 8)
		upLines[player] = 0;

	// フィールド消去
	for(i = 0; i < 220; i++) {
		fld[i + player * 220] = 0;
		fldt[i + player * 220] = 0;		// #1.60c7j5
		fldi[i + player * 220] = 0;		// #1.60c7n1
		fldu[i + player * 220] = 0;
		flds[i + player * 220] = 0;
		fldubuf[i + player * 220] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.02 - モードセレクト画面
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：これが1になるとゲーム開始
// statc[player * 10 + 1]：カウンタ
// statc[player * 10 + 2]：0=ルールセレクト 1=モードセレクト 2=その他
// statc[player * 10 + 3]：その他画面でのカーソル位置

void statSelectMode(int player) {
	int i,ranktmp,j,twg_opt[2];

	//statc[player * 10 + 1]++;
	if(statc[player * 10 + 4] > 0){
		statc[player * 10 + 4]--;
		if(statc[player * 10 + 4] == 0) PlaySE(5);
	} else if(statc[player * 10 + 4] < 0){
		statc[player * 10 + 4]++;
		if(statc[player * 10 + 4] == 0) PlaySE(5);
	}else
		statc[player * 10 + 1]++;

	padRepeat(player);
	padRepeat2(player);

	// モードセレクト曲
	if( (!IsPlayMIDI()) && (wavebgm == 0) ) {
		PlayMIDI();
	} else if(((stat[1 - player] == 0) || (stat[1 - player] == 10)) && (!IsPlayWave(62)) && (wavebgm >= 1) ) {
		PlayWave(62);
	}


	printSMALLFont(124 + 192 * player - 96 * maxPlay, 45, "Select", 4);

	if(statc[player * 10 + 2] == 0) {
		// ルールセレクト
		printSMALLFont(124 + 192 * player - 96 * maxPlay, 54, "   Rot?Rule", 4);
		printFont(15 + 24 * player - 12 * maxPlay, 12, "<        >", count % 9);

		ExBltRect(77, 120 + 192 * player -96 * maxPlay , 66,  (statc[player * 10 + 1] % 40) * 3, 28, 30, 8);
		ExBltRect(77, 120 + 192 * player -96 * maxPlay , 75,  160 - ((statc[player * 10 + 1] % 40) * 3), 20, 30, 8);

		ExBltRect(77, 170 + 192 * player -96 * maxPlay , 66,  50 + ((statc[player * 10 + 1] % 40) * 3), 28, 30, 8);
		ExBltRect(77, 170 + 192 * player -96 * maxPlay , 75,  210 - ((statc[player * 10 + 1] % 40) * 3), 20, 30, 8);

		if(statc[player * 10 + 4] < 0){
			ExBltRect(55,128 + ((16+statc[player * 10 + 4]) * 4) + 192 * player -96 * maxPlay , 84,
				(64*(rots[player]+1))*(rots[player]<8),32*(fontc[(rots[player]+1)*(rots[player]<8)]),
				(statc[player * 10 + 4]* -4),32);
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 84,
				(64*rots[player])+(statc[player * 10 + 4] * -4),32*fontc[rots[player]],((16+statc[player * 10 + 4])* 4),32);
		} else if(statc[player * 10 + 4] > 0){
			ExBltRect(55,128 + (statc[player * 10 + 4] * 4) + 192 * player -96 * maxPlay , 84,
				(64*rots[player]),32*fontc[rots[player]],((16-statc[player * 10 + 4])* 4),32);
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 84,
				(64*(rots[player]-1))+576*(rots[player]==0)+((16-statc[player * 10 + 4]) * 4),
				32*fontc[rots[player]-1+(9*(rots[player]==0))],(statc[player * 10 + 4] * 4),32);
		} else ExBltRect(55,128 + 192 * player -96 * maxPlay , 84, 64*rots[player] ,32*fontc[rots[player]],64,32);

			ExBltRect(55,128 + 192 * player -96 * maxPlay , 64, 64*rots[player] ,384,64,21);

			ExBltRect(87,208 + 48 * player -96 * maxPlay , 40, (statc[player * 10 + 1]%6)*48 ,(statc[player * 10 + 1]%30/6)*48,48,48);

			if(statc[player * 10 + 4] == 0){
				j = (statc[player * 10 + 1]-16) % 420 / 60;
				if(isWRule(player)){
					i = wcol[j];
				} else if((rots[player] >= 4) && (rots[player] != 8)){
					i = acol[j];
				} else{
					i = ccol[j];
				}
				drawBlockFast(27+ 6 * player - 12 * maxPlay,6 + isWRule(player) * (j != 0) + ((j == 0) && (rots[player] != 8)), j, 0, i, 0, 0, 1, player, (disable_giji3D < 3));
			}
		if(getDrawRate() != 1){
			//スクロールする回転の説明
			ExBltRect(84,120 + 192 * player -96 * maxPlay , 118, statc[player * 10 + 1] % 320 ,(20*rots[player]) + (10 * (english)), 80, 10);
		}
		//レバーとボタンの画像
		ExBltRect(25,126 + 192 * player -96 * maxPlay , 128, 224 ,0+(40*((isWRule(player)) && (w_reverse)))+(80*(rots[player] >= 7)),68,40);
		if(!maxPlay) ExBltRect(55,211, 98 ,64*rots[player],416,64,32);
		if(!english)	//落下特性（日本語）
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 168, 64*rots[player] ,320,64,32);
		else			//落下特性（English）
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 168, 64*rots[player] ,352,64,32);
	} else if(statc[player * 10 + 2] == 1) {
		// モードセレクト
		printSMALLFont(124 + 192 * player - 96 * maxPlay, 54, "  Game Mode", 4);

		ExBltRect(77, 120 + 192 * player -96 * maxPlay , 63,  (count % 40) * 3, 28, 80, 8);
		ExBltRect(77, 120 + 192 * player -96 * maxPlay , 160,  160 - ((count % 40) * 3), 20, 80, 8);

		// カーソル
		printFont(15 + 24 * player - 12 * maxPlay, 9 + (gameMode[player] - ((gameMode[player] >= 6 ) +(gameMode[player] >= 9) )*2-(gameMode[player] >= 10))*2, "b", fontc[rots[player]]);
		printFont(14 + 24 * player - 12 * maxPlay, 9 + (gameMode[player] - ((gameMode[player] >= 6 ) +(gameMode[player] >= 9) )*2-(gameMode[player] >= 10))*2, "<          >", count % 9);

		if(gameMode[player]>=9){
			printFont(15 + 24 * player - 12 * maxPlay, 8, "k        k", count % 9);//↑カーソル
		}
		if(gameMode[player]<10){
			printFont(15 + 24 * player - 12 * maxPlay, 20, "n        n", count % 9);//↓カーソル
		}
		//biginner
		if(gameMode[player]<9){
			ExBltRect(86, 130+192 * player -96 * maxPlay , 71 , 72 * (novice_mode[player]), 0, 72, 9);//BEGINNERの文字
			if(statc[player * 10 + 2] == 1)	{
				if(novice_mode[player]){
					ExBltRect(85, 144+192 * player -96 * maxPlay, 80, 0, 7*20, 70, 7);
				}else{
					ExBltRect(85, 144+192 * player -96 * maxPlay, 80, 0, 7*21, 70, 7);
				}
			}
		}
		//master
		if(gameMode[player]<10){
			ExBltRect(86, 130+192 * player -96 * maxPlay , 87-(gameMode[player]==9)*16 , 72 * enable_grade[player], 9, 72, 9);//MASTER
			if(((gameMode[player] >= 1) && (gameMode[player] <= 3)) && (item_mode[player])){
				if(statc[player * 10 + 2] == 1)
					ExBltRect(85, (18 + 24 * player - 12 * maxPlay)*8, (10 + (gameMode[player] - ((gameMode[player] >= 6)+(gameMode[player] >= 9)) * 2)*2)*8, 0, 7*22, 50, 7);
				else
					printFont(19 + 24 * player - 12 * maxPlay, 19 , "ITEM", 5);
			}
			if(((gameMode[player] == 1) || (gameMode[player] == 2)) && (hebo_plus[player])){
				if(statc[player * 10 + 2] == 1)
					ExBltRect(85, (18 + 24 * player - 12 * maxPlay)*8, (10 + (gameMode[player] - ((gameMode[player] >= 6)+(gameMode[player] >= 9)) * 2)*2)*8, 0, 7*23, 50, 7);
				else
					printFont(17 + 24 * player - 12 * maxPlay, 19 , "HEBO+", 9);
			}
		}
		//20G
		ExBltRect(86, 130+192 * player -96 * maxPlay , 103-(gameMode[player]>=9)*16 -(gameMode[player]==10)*16, 72 * enable_grade[player], 18, 72, 9);//20G
		//devil
		ExBltRect(86, 130+192 * player -96 * maxPlay , 119-(gameMode[player]>=9)*16 -(gameMode[player]==10)*16, 72 * devil_minus[player], 27, 72, 9);
		if((gameMode[player] == 3) && (death_plus[player])){
			if(statc[player * 10 + 2] == 1)
				ExBltRect(85, (19 + 24 * player - 12 * maxPlay)*8, (10 + (gameMode[player] - (gameMode[player] >= 6) * 2)*2)*8, 0, 7*24, 40, 7);
			else
				printFont(19 + 24 * player - 12 * maxPlay, 19 , "DEATH+", 2);
		}
		//tomoyo
		ExBltRect(86, 130+192 * player -96 * maxPlay , 135-(gameMode[player]>=9)*16 -(gameMode[player]==10)*16, 72 * tomoyo_opt[player], 54, 72, 9);
		if((gameMode[player] == 6) && (tomoyo_opt[player] == 4) && (fpbas_mode[player])){
			if(statc[player * 10 + 2] == 1)
				ExBltRect(85, (21 + 24 * player - 12 * maxPlay)*8, (10 + (gameMode[player] - ((gameMode[player] >= 6)+(gameMode[player] >= 9)) * 2)*2)*8, 70, 7*8, 30, 7);
			else if(statc[player * 10 + 2] == 2)
				printFont(15 + 24 * player - 12 * maxPlay, 19 , "BASIC", 3);
		}
		if((gameMode[player] == 6) && (tomoyo_opt[player] == 4) &&(!fpbas_mode[player])){
			if(statc[player * 10 + 2] == 1)
				ExBltRect(85, (21 + 24 * player - 12 * maxPlay)*8, (10 + (gameMode[player] - ((gameMode[player] >= 6)+(gameMode[player] >= 9)) * 2)*2)*8, 70, 7*7, 30, 7);
			else if(statc[player * 10 + 2] == 2)
				printFont(15 + 24 * player - 12 * maxPlay, 19 , "18MIN", 3);
		}
		//ace
		ExBltRect(86, 130+192 * player -96 * maxPlay , 151-(gameMode[player]>=9)*16-(gameMode[player]==10)*16 , 72 * anothermode[player], 63, 72, 9);
		//std
		if(gameMode[player]>=9){
			ExBltRect(86, 130+192 * player -96 * maxPlay , 151 -(gameMode[player]==10)*16, 72 * std_opt[player], 81, 72, 9);
			ExBltRect(85, (18 + 24 * player - 12 * maxPlay)*8, (20 -(gameMode[player] >= 10)* 2)*8, 0, 7*(25 + std_opt[player]), 50, 7);
			if(relaymode[player]){
				if(statc[player * 10 + 2] == 1)
					printFontEx(15 + 24 * player - 12 * maxPlay, 143, "ROT.RELAY", 7);
				else if(statc[player * 10 + 2] == 2)
					printFont(15 + 24 * player - 12 * maxPlay, 19 , "ROT.RELAY", 7);
			}
		}
		//ori
		if(gameMode[player]==10){
			ExBltRect(86, 130+192 * player -96 * maxPlay , 151 , 72 * ori_opt[player], 90, 72, 9);
			ExBltRect(85, 150+192 * player -96 * maxPlay, 162, 70, 7*(2+ori_opt[player]), 35, 7);
		}

		// テキストガイド
		if(textguide){
			printFont(15+24 * player - 12 * maxPlay, 26, "<L R>:TYPE", count % 9);
			if(gameMode[player] == 6){
				printFont(26 + 10 * player - 12 * maxPlay, 15, "C:", 0);
				printFont(26 + 6 * player - 12 * maxPlay, 16, "RANDOM", 7);
			}else{
				printFont(26 + 10 * player - 12 * maxPlay, 15, "C:", 0);
				printFont(26 + 9 * player - 12 * maxPlay, 16, "BIG", 7);
			}
			if(gameMode[player] == 3){
				printFont(26 + 7 * player - 12 * maxPlay, 17,"HOLD:", 0);
				if(!death_plus[player])
					printFont(26 + 6 * player - 12 * maxPlay, 18, "DEATH+", 2);
				if(!item_mode[player])
					printFont(26 + 8 * player - 12 * maxPlay, 19-(death_plus[player]),"ITEM", 5);
			}
			if((gameMode[player] == 1) || (gameMode[player] == 2)){
				printFont(26 + 7 * player - 12 * maxPlay, 17,"HOLD:", 0);
				if(!item_mode[player])
					printFont(26 + 8 * player - 12 * maxPlay, 18,"ITEM", 5);
				if(!hebo_plus[player])
					printFont(26 + 7 * player - 12 * maxPlay, 19-(item_mode[player]),"HEBO+", 9);
			}

			if((gameMode[player] == 6)&&(tomoyo_opt[player]==4)){
				printFont(26 + 7 * player - 12 * maxPlay, 17,"HOLD:", 0);
				if(!fpbas_mode[player])
					printFont(26 + 7 * player - 12 * maxPlay, 18,"BASIC", 3);
				else
					printFont(26 + 7 * player - 12 * maxPlay, 18,"18MIN", 3);
			}
			if((gameMode[player] == 9) && (std_opt[player] < 2)){
				printFont(26 + 7 * player - 12 * maxPlay, 17,"HOLD:", 0);
				if(!relaymode[player]){
					printFont(26 + 6 * player - 12 * maxPlay, 18, "ROT.", 7);
					printFont(26 + 6 * player - 12 * maxPlay, 19, "RELAY", 7);
				}
			}
			if(dispnextkey[player] == 0x3D){
				printFont(24 + 9 * player - 12 * maxPlay, 0, "F3:", 0);
				printFont(27 + 8 * player - 12 * maxPlay, 0, "NEXT", 0);
			}
		}

		// モード説明文
		if ( getDrawRate() == 1 ){
			if((gameMode[player] >= 6) && (gameMode[player] != 10)) {
				ExBltRect(25, 119 + player * 192 - 96 * maxPlay, 168, 0+(80*(english)), (gameMode[player] - 2) * 32, 80, 32);
			} else if(gameMode[player] != 10) {
				ExBltRect(25, 119 + player * 192 - 96 * maxPlay, 168, 0+(80*(english)), gameMode[player] * 32, 80, 32);
			}

		}else{
			if((gameMode[player] >= 6) && (gameMode[player] != 10)) {
				ExBltRect(25, 119 + player * 192 - 96 * maxPlay, 168, 0+(80*(english)), (gameMode[player] - 2) * 32, 80, 32);
			} else if(gameMode[player] != 10) {
				ExBltRect(25, 119 + player * 192 - 96 * maxPlay, 168, 0+(80*(english)), gameMode[player] * 32, 80, 32);
			}
			if(gameMode[player] == 0){
				ExBltRect(25, 120 + player * 192 - 96 * maxPlay, 192,statc[player * 10 + 1] % 320,264 + (16 * novice_mode[player]) + (8 * english),80,8);
			}else if(gameMode[player] <= 2){
				ExBltRect(25, 120 + player * 192 - 96 * maxPlay, 192,statc[player * 10 + 1] % 320,296 + (80 * (gameMode[player] - 1)) +(16 * enable_grade[player]) + (8 * english),80,8);
			}else if(gameMode[player] == 3){
				ExBltRect(25, 120 + player * 192 - 96 * maxPlay, 192,statc[player * 10 + 1] % 320,456 + (16 * devil_minus[player]) + (8 * english),80,8);
			}else if(gameMode[player] == 7) {
				ExBltRect(25, 119 + player * 192 - 96 * maxPlay, 192, 160, ((gameMode[player] - 2)*32)+(8*anothermode[player])+(32*(english)), 80, 8);
			} else if(gameMode[player] == 6) {
				if(tomoyo_opt[player] == 4)
					ExBltRect(25, 119 + player * 192 - 96 * maxPlay, 192, 192, 128, 28, 8);//FPの文字
				else
					ExBltRect(25, 119 + player * 192 - 96 * maxPlay, 192, 160, 128+(tomoyo_opt[player]*8), 32, 8);
			}
		}
	}

	else if(statc[player * 10 + 2] == 2) {
		// その他
		printSMALLFont(124 + 192 * player - 96 * maxPlay, 54, "    Others", 4);

		printFont(15 + 24 * player - 12 * maxPlay, 10 + (statc[player * 10 + 3] * 2), "b", fontc[rots[player]]);

		// SHIRASE
		printFont(15 + 24 * player - 12 * maxPlay, 9, "LINE UP", fontc[rots[player]]);
		if(p_shirase[player]) {
			sprintf(string[0], "ON");
		} else {
			sprintf(string[0], "OFF");
		}
		printFont(16 + 24 * player - 12 * maxPlay, 10, string[0], count % 9 * (statc[player * 10 + 3] == 0));

		// UPLINE
		printFont(15 + 24 * player - 12 * maxPlay, 11, "UPLINE", fontc[rots[player]]);
		if(upLineT[player]) {
			sprintf(string[0], "PATTERN");
		} else {
			sprintf(string[0], "COPY");
		}
		printFont(16 + 24 * player - 12 * maxPlay, 12, string[0], count % 9 * (statc[player * 10 + 3] == 1));

		// HIDDEN
		printFont(15 + 24 * player - 12 * maxPlay, 13, "HIDDEN", fontc[rots[player]]);
		sprintf(string[0], "LV %d", hidden[player]);
		if(hidden[player] <= 7){
			printFont(16 + 24 * player - 12 * maxPlay, 14, string[0], count % 9 * (statc[player * 10 + 3] == 2));
		}else if(hidden[player] == 8){
			printFont(16 + 24 * player - 12 * maxPlay, 14, "M", count % 9 * (statc[player * 10 + 3] == 2));
		}else if(hidden[player] == 9){
			printFont(16 + 24 * player - 12 * maxPlay, 14, "UNDER M3", count % 9 * (statc[player * 10 + 3] == 2));
		}else if(hidden[player] == 10){
			printFont(16 + 24 * player - 12 * maxPlay, 14, "UNDER M2", count % 9 * (statc[player * 10 + 3] == 2));
		}else if(hidden[player] == 11){
			printFont(16 + 24 * player - 12 * maxPlay, 14, "UNDER M1", count % 9 * (statc[player * 10 + 3] == 2));
		}
	}


	// ↑
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 0) ) {
		PlaySE(5);

		// モードセレクト
		if(statc[player * 10 + 2] == 1) {
			statc[player * 10 + 1] = 0;
			gameMode[player]--;
			death_plus[player] = 0;
			item_mode[player] = 0;
			hebo_plus[player] = 0;
			fpbas_mode[player] = 0;
			heboGB[player] = 0;
			relaymode[player] = 0;
			if(gameMode[player] < 0)
				gameMode[player] = 10;

			if( (gameMode[player] == 4) || (gameMode[player] == 5) )
				gameMode[player] = 3;
			if(gameMode[player] == 8)
				gameMode[player] = 7;
		}
		// その他
		if(statc[player * 10 + 2] == 2) {
			statc[player * 10 + 3]--;
			if(statc[player * 10 + 3] < 0)statc[player * 10 + 3] = 2;
		}
	}

	// ↓
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 1) ) {
		PlaySE(5);

		// モードセレクト
		if(statc[player * 10 + 2] == 1) {
			statc[player * 10 + 1] = 0;
			gameMode[player]++;
			death_plus[player] = 0;
			item_mode[player] = 0;
			hebo_plus[player] = 0;
			fpbas_mode[player] = 0;
			heboGB[player] = 0;
			relaymode[player] = 0;
			if(gameMode[player] > 10)
				gameMode[player] = 0;

			if( (gameMode[player] == 4) || (gameMode[player] == 5) )
				gameMode[player] = 6;
			if(gameMode[player] == 8)
				gameMode[player] = 9;
		}
		// その他
		if(statc[player * 10 + 2] == 2) {
			statc[player * 10 + 3]++;
			if(statc[player * 10 + 3] > 2)statc[player * 10 + 3] = 0;
		}
	}

	// ←
	if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) )
	if( getPressState(player, 2) ) {
		PlaySE(3);
		// ルールセレクト
		if(statc[player * 10 + 2] == 0) {
			rots[player]--;
			if(rots[player] < 0) rots[player] = 8;
			statc[player * 10 + 4] = -16;
			statc[player * 10 + 1] = 0;
			setNextBlockColors(player, 1);
		}
		if(statc[player * 10 + 2] == 1) {
			statc[player * 10 + 1] = 0;
			//BEGIN
			novice_mode[player]=!novice_mode[player];
			//MAS
			enable_grade[player]--;
			if(enable_grade[player] < 1)
				enable_grade[player] = 4;
				//DEVIL
			devil_minus[player] = !devil_minus[player];
			//ACE
			anothermode[player]--;
			if(anothermode[player] < 0)
				anothermode[player] = 3;
			//TOMOYO
			tomoyo_opt[player]--;
			fpbas_mode[player]=0;
			if(tomoyo_opt[player] < 0)
				tomoyo_opt[player] = 4;
			//STANDARD
			std_opt[player]--;
			if(std_opt[player] < 0)
				std_opt[player] = 3;
			if(std_opt[player] >= 2)
				relaymode[player] = 0;
				//ORIGINAL
			ori_opt[player]--;
			if(ori_opt[player] < 0)
				ori_opt[player] = 3;
		}
		if(statc[player * 10 + 2] == 2) {
			if(statc[player * 10 + 3] == 0) {
				p_shirase[player] = !p_shirase[player];
			}
			// UPLINE
			if(statc[player * 10 + 3] == 1) {
				upLineT[player] = !upLineT[player];
			}
			// HIDDEN
			if(statc[player * 10 + 3] == 2) {
				hidden[player]--;
				if(hidden[player] < 0) hidden[player] = 11;
			}
		}
	}

	// →
	if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) )
	if( getPressState(player, 3) ) {
		PlaySE(3);
		// ルールセレクト
		if(statc[player * 10 + 2] == 0) {
			rots[player]++;
			if(rots[player] > 8) rots[player] = 0;
			statc[player * 10 + 4] = 16;
			setNextBlockColors(player, 1);
			statc[player * 10 + 1] = 0;
		}
		if(statc[player * 10 + 2] == 1) {
			statc[player * 10 + 1] = 0;
			//BEGIN
			novice_mode[player]=!novice_mode[player];

			//MAS
			enable_grade[player]++;
			if(enable_grade[player] > 4)
				enable_grade[player] = 1;
			//DEVIL
			devil_minus[player] = !devil_minus[player];
			anothermode[player]++;
			if(anothermode[player] > 3)
				anothermode[player] = 0;

			tomoyo_opt[player]++;
			fpbas_mode[player]=0;
			if(tomoyo_opt[player] > 4)
				tomoyo_opt[player] = 0;

			std_opt[player]++;
			if(std_opt[player] > 3)
				std_opt[player] = 0;
			if(std_opt[player] >= 2)
				relaymode[player] = 0;
			ori_opt[player]++;
			if(ori_opt[player] > 3)
				ori_opt[player] = 0;
		}
		if(statc[player * 10 + 2] == 2) {
			if(statc[player * 10 + 3] == 0) {
				p_shirase[player] = !p_shirase[player];
			}
			// UPLINE
			if(statc[player * 10 + 3] == 1) {
				upLineT[player] = !upLineT[player];
			}
			// HIDDEN
			if(statc[player * 10 + 3] == 2) {
				hidden[player]++;
				if(hidden[player] > 11) hidden[player] = 0;
			}
		}
	}


	// Cボタン
	//if(statc[player * 10 + 2] != 0){
	if( getPressState(player, 6) ) {
		if(statc[player * 10 + 2] == 1){
		if(gameMode[player] == 6)//RANDOM
			ExBltRect(85, (16 + 24 * player - 12 * maxPlay)*8, (10 + (gameMode[player] - ((gameMode[player] >= 6)+(gameMode[player] >= 9)) * 2)*2)*8, 70, 7*6, 33, 7);
		else//BIG
			ExBltRect(85, (16 + 24 * player - 12 * maxPlay)*8, (10 + (gameMode[player] - ((gameMode[player] >= 6)+(gameMode[player] >= 9)) * 2)*2)*8, 0, 7*1, 20, 7);
		}
		else if(statc[player * 10 + 2] == 2){
		if(gameMode[player] == 6)
			printFont(15 + 24 * player - 12 * maxPlay, 19 , "RANDOM", 7);
		else
			printFont(15 + 24 * player - 12 * maxPlay, 19 , "BIG", 7);
		}
	}
	// HOLDボタン
	// アイテムモードとか、DEATH+とか
	if(getPushState(player,7) && (statc[player * 10 + 2] == 1)) {
		PlaySE(5);
		if((gameMode[player] == 6)&&(tomoyo_opt[player]==4)){
			if(!fpbas_mode[player])
			fpbas_mode[player]=1;
			else if(fpbas_mode[player])
			fpbas_mode[player]=0;
		}
		if((gameMode[player] == 1) || (gameMode[player] == 2)){
			if((!item_mode[player]) && (!hebo_plus[player]))
				item_mode[player] = 1;
			else if(item_mode[player]){
				item_mode[player] = 0;
				hebo_plus[player] = 1;
				if((gameMode[player] ==1) || (gameMode[player] == 2)){//せり上がりパターンをモードで決定
					upLineT[player]=1;//パターン
				}else{
					upLineT[player]=0;//コピー
				}
			}else
				hebo_plus[player] = 0;
		}
		if(gameMode[player] == 3){	//DEATH+
			if((!death_plus[player]) && (!item_mode[player])){
				item_mode[player]=0;
				death_plus[player]=1;
			}else if(death_plus[player]){
				item_mode[player]=1;
				death_plus[player]=0;
			}else if(item_mode[player]){
				item_mode[player]=0;
				death_plus[player]=0;
			}
		}
		if((gameMode[player] == 9) && (std_opt[player] <= 1))
			relaymode[player] = !relaymode[player];
	}

	// Bボタン
	if( getPushState(player, 5) ) {
		PlaySE(5);
		death_plus[player] = 0;
		item_mode[player] = 0;
		hebo_plus[player] = 0;
		devil_minus[player] = 0;
		squaremode[player] =0;
		heboGB[player] = 0;
		relaymode[player] = 0;

		statc[player * 10 + 2]--;
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 4] = 0;

		// フィールド消去 #1.60c7s6
		for(i = 0; i < 220; i++) {
			fld[i + player * 220] = 0;
			fldt[i + player * 220] = 0;
			fldi[i + player * 220] = 0;
			fldu[i + player * 220] = 0;
			flds[i + player * 220] = 0;
			fldubuf[i + player * 220] = 0;
		}

		if(statc[player * 10 + 2] < 0) {
			stat[player] = 0;
			if(stat[1 - player] == 0) flag = 1;
		}
	}

	// Aボタン
	if( getPushState(player, 4) ) {
		PlaySE(10);
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 4] = 0;

		// 次のページへ
		if(statc[player * 10 + 3] != 5){
		statc[player * 10 + 2]++;
		}
		// ゲーム開始へ
		if(statc[player * 10 + 2] > (2 - ((mini_select) || (gameMode[player] == 6)) )) {
			statc[player * 10 + 0] = 1;
		}

		if(statc[player * 10 + 0]) {
			// 開始処理
			if(upLineT[player] >= 2) upLineT[player] = 0;	// せり上がりがRANDOMになっていたらSHIRASEにする
			if(upLineT[player] == -1) upLineT[player] = 0;
			StopAllBGM();	// 音楽停止
			backno = 0;		// 背景初期化

			// 開始スピード設定
			start[player] = 0;
			setStartLevel(player);

			// mini_selectが1なら設定を固定
			if(mini_select) {

				p_shirase[player] = (gameMode[player] == 3);
				if(((gameMode[player] == 3) && (!devil_minus[player])) || ((gameMode[player] == 7) && (anothermode[player] == 2))) {
					PlaySE(18);//歓声
					gflash[player]=120;
					upLineT[player] = 0;
				}
			}

			//devil-だとせり上がりなし
			if(((enable_grade[player] == 1) && (repversw < 42)) || ((devil_minus[player])&&(gameMode[player] == 3)&&(repversw >= 30))){
				p_shirase[player] = 0;
			}
			//横選択で他のモードも決定できてしまうためリセット
			if(gameMode[player]!=0){
			novice_mode[player] = 0;
			}
			if(gameMode[player]!=3){
			devil_minus[player] = 0;
			}
			if(gameMode[player]!=6){
			fpbas_mode[player] = 0;
			}

			if( getPressState(player, 6) ) {
				if(gameMode[player] == 6) {
					// ランダムモード
					randommode[player] = 1;
				} else {
					// ビッグモード
					IsBig[player] = 1;
					IsBigStart[player] = 1;
				}
			} else {
				randommode[player] = 0;
				IsBig[player] = 0;
				IsBigStart[player] = 0;
			}

			if( (!mini_select) && (gameMode[player] == 6) ) {
				// TOMOYOセレクト
				if(!randommode[player]) {
					versusInit(player);
				}

				stat[player] = 19;
				statc[player * 10 + 0] = 0;
				statc[player * 10 + 1] = 0;
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;
			} else if( (debug) && ((gameMode[player] <= 3)||(gameMode[player] == 10)) ) {
				// レベルセレクト
				stat[player] = 12;
				statc[player * 10 + 0] = 0;
				statc[player * 10 + 1] = 0;
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;
			} else if(gameMode[player] == 9){
				if(std_opt[player]<=1){
					// レベルセレクト
					stat[player] = 37;
					statc[player * 10 + 0] = 0;
					statc[player * 10 + 1] = 0;
					statc[player * 10 + 2] = 0;
					statc[player * 10 + 3] = 0;
				}else if(std_opt[player]==2){
					squaremode[player] =1;
					stat[player] = 3;
					statc[player * 10 + 0] = 0;
					statc[player * 10 + 1] = 0;
					statc[player * 10 + 2] = 0;
					statc[player * 10 + 3] = 0;
				}else {
					stat[player] = 3;
					statc[player * 10 + 0] = 0;
					statc[player * 10 + 1] = 0;
					statc[player * 10 + 2] = 0;
					statc[player * 10 + 3] = 0;
				}
			}  else {
				if(gameMode[player] == 6) {
					// TOMOYOの場合はツモリセット
				//	versusInit(player);

					// ステージ設定
					if(tomoyo_opt[player] == 0){			//TI
						ltime[player] = 180 * 60;
						start_stage[player] = 0;
					}else if(tomoyo_opt[player] == 1){	//EH
						ltime[player] = 600 * 60;
						start_stage[player] = 27;
					}else if(tomoyo_opt[player] == 2)	//TGT
						start_stage[player] = 45;
					else if(tomoyo_opt[player] == 3)	//Edit
						start_stage[player] = 72;
					else{								//FP
						ltime[player] = 1080 * 60;
						start_stage[player] = 100;
					}
				versusInit(player);
				stage[player] = start_stage[player];
			}else
				if(((gameMode[player] == 1) || (gameMode[player] == 2))){
					if((((enable_randexam==1) && (!item_mode[player]) && (!hebo_plus[player])) && (Rand(10) < 2)) ||(getPressState(player,7))){
						item_mode[player] = 0;
						hebo_plus[player] = 0;
						examination[player] = 1;
					}
					//admitgradecheck(player);
					if((enable_randexam==2) && (!item_mode[player]) && (!hebo_plus[player])&&(Admitgradecheck(player)>0)&&(!cp_player_1p)){
						item_mode[player] = 0;
						hebo_plus[player] = 0;
						examination[player] = Admitgradecheck(player);
					}
				}
				// ゲーム開始
				stat[player] = 3;				// Ready
				statc[player * 10 + 0] = 0;		// ステータスカウンタを0に
				statc[player * 10 + 1] = 0;
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;

				if(((gameMode[player] == 1) || (gameMode[player] == 2)) && (examination[player]>0)){

					if((enable_randexam==1)||((enable_randexam==2)&&(enable_grade[player]!=4))){
						//エラー対策
						//指定されたレンジからずれていたら最も近い小さな値を入れる
						for(i=0;i<2;i++){//
							if(enable_grade[player]==1){
								//18~31だったら17に戻す
								if((exam_range[((enable_grade[player] - 1) * 2) + i] >= 18)&&(exam_range[((enable_grade[player] - 1) * 2) +i] <= 31))
								exam_range[(enable_grade[player] - 1) * 2 + i]=17;//S9
							}else if(enable_grade[player]==3){
								//18~26だったら17に戻す
								if((exam_range[((enable_grade[player] - 1) * 2)+i] >= 18)&&(exam_range[((enable_grade[player] - 1) * 2) + i] <= 26))
								exam_range[((enable_grade[player] - 1) * 2) + i]=17;//S9
								//28~31だったら27に戻す
								if((exam_range[((enable_grade[player] - 1) * 2) + i] >= 28)&&(exam_range[((enable_grade[player] - 1) * 2) + i] <= 31))
								exam_range[((enable_grade[player] - 1) * 2) + i]=27;//m
							}
							//33以上だったら32に戻す
							if(exam_range[((enable_grade[player] - 1) * 2) + i] > 33)
							exam_range[((enable_grade[player] - 1) * 2)+ i]=32;//Gm
							//0以下だったら0に戻す
							if(exam_range[((enable_grade[player] - 1) * 2)+ i] < 0)
							exam_range[((enable_grade[player] - 1) * 2)+ i]=0;//0
						}

						//min>maxとなっていたらmin=maxとする(minにmaxを入れる)
						if(exam_range[(enable_grade[player] - 1) * 2]>exam_range[((enable_grade[player] - 1) * 2) + 1])
						exam_range[(enable_grade[player] - 1) * 2]=exam_range[((enable_grade[player] - 1) * 2) + 1];

						//段位認定試験発生
						do{	//試験段位を設定
							exam_grade[player] = Rand(32) + 1;
						}while((exam_grade[player] < exam_range[(enable_grade[player] - 1) * 2]) ||
						(exam_grade[player] > exam_range[((enable_grade[player] - 1) * 2) + 1]));
					}

					stat[player] = 30;				// 試験っぽいの
					statc[player * 10 + 0] = 0;		// ステータスカウンタを0に
					statc[player * 10 + 1] = 0;		// 試験開始宣言
					statc[player * 10 + 2] = exam_grade[player];
					statc[player * 10 + 3] = 0;
				}
				if((gameMode[player] == 6) && (start_stage[player] >= 100)){
					stat[player] = 36;				// F-Pointセレクト
					statc[player * 10] = 0;
					statc[player * 10 + 1] = 0;
					statc[player * 10 + 2] = 0;
					statc[player * 10 + 3] = 0;
				}
			}
			// BGM設定
			bgmlv = setstartBGM(gameMode[player], player);
			fadelv[player] = 0;
		}
	}
}
//試験する段位を判定
//0;なし
//1:昇段
//2:降格
int Admitgradecheck(int player){
	int i,j,temp1[2],temp2[2],grade_his_buf[5],sort_grade[3];
	if(gameMode[player]!=1)return 0;
	if(enable_grade[player]!=4)return 0;
	for(i=0;i<5;i++){
		if((grade_his[i]<0)||(grade_his[i]>32))
		return 0;
	}
	//履歴並べ替え作業
	for(i = 0 ; i < 5 ; i++){
		grade_his_buf[i] = grade_his[i];//バッファに入れる
	}
	//バブルソートしかしらない
	for (i = 0; i < 4; i++) {
        	for (j = 4; j > i; j--) {
        	    if (grade_his_buf[j - 1] > grade_his_buf[j]) {  /* 前の要素の方が大きかったら */
        	        temp1[player] = grade_his_buf[j];        /* 交換する */
        	        grade_his_buf[j] = grade_his_buf[j - 1];
        	        grade_his_buf[j - 1]= temp1[player];
        	    }
        	}
    	}
    	for(i = 0 ; i < 3 ; i++){
		sort_grade[i] = grade_his_buf[i];//並べ替え
	}
	temp1[player]=grade_his[0]+grade_his[1]+grade_his[2];
	temp2[player]=5-grade_pasttime[player];//試験から経過時間
	if(admit_grade[player]<32){

		//昇格
		temp2[player]=5-grade_pasttime[player];
		if(temp2[player]<1)temp2[player]=1;//0にはしない
		if((grade_his[0] == grade_his[1])&&((admit_grade[player]-grade_his[0]) > temp2[player])){
			exam_grade[player]=grade_his[0];
			return 1;
		}

		temp1[player]=grade_his[0]+grade_his[1]+grade_his[2]/3;
		temp2[player]=4-grade_pasttime[player];//試験から経過時間
		if(temp2[player]<1)temp2[player]=1;//0にはしない
		if((temp1[player]%3==0)&&((admit_grade[player]-grade_his[0]) > temp2[player])){
			exam_grade[player]=temp1[player];
			return 1;
		}
		temp1[player]=grade_his[0]+grade_his[1]+grade_his[2]+ grade_his[3]/3;
		temp2[player]=3-grade_pasttime[player];//試験から経過時間
		if(temp2[player]<1)temp2[player]=1;//0にはしない
		if((2 <= grade_pasttime[player])&&(temp1[player] == 0)&&((admit_grade[player]-temp1[player]) > temp2[player])){
			exam_grade[player]=temp1[player];
			return 1;
		}
	}
	if(admit_grade[player]>0){
	//降格条件
	//認定段位が高くなると落ちやすくなる
		if(admit_grade[player] > 18){//m8以上
			exam_grade[player]=grade_his[0];
			return 2;
		}
		if(admit_grade[player] > 24){//m8以上
			if((grade_his[0] + grade_his[1]) / 2 < admit_grade[player] - 3){
				exam_grade[player]=grade_his[0];
				return 2;
			}
		}

		if(admit_grade[player] > 18){//m1以上
			if((grade_his[0]+grade_his[1]+grade_his[2]) / 3 < admit_grade[player]-4){
				exam_grade[player]=grade_his[0];
				return 2;
			}
		}
		//大きいほうから3つの平均が現在より5つ下
		if(admit_grade[player] > 10){
			if((grade_his[0]+grade_his[1]+grade_his[2]+grade_his[3]+grade_his[4]) / 3 < admit_grade[player]-5){
				exam_grade[player]=sort_grade[0];
				return 2;
			}
		}
	}

	return 0;//何もなし
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.37 -レベルセレクト
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statSelectStandardSp(int player) {
	int	i;

	//statc[player * 10 + 1]++;

//	printFont(16 + 24 * player - 12 * maxPlay, 6, "SELECT", 4);
//	printFont(19 + 24 * player - 12 * maxPlay, 7, "WAITS", 4);
	printSMALLFont(124 + 192 * player - 96 * maxPlay, 45, "Select", 4);
	printSMALLFont(124 + 192 * player - 96 * maxPlay, 54, "    Waits", 4);

	printFont(15 + 24 * player - 12 * maxPlay, 9, "SP PRESET", 6);
	printFont(15 + 24 * player - 12 * maxPlay, 10, waitname[statc[player * 10 + 1]], count % 9 * (statc[player * 10 ]==0));

	printFont(15 + 24 * player - 12 * maxPlay, 12, "WAITS", 6);
	sprintf(string[0], "ARE: %d",  wait1[player]);
	printFont(15 + 24 * player - 12 * maxPlay, 13, string[0], count % 9  * (statc[player * 10 ]==1));
	sprintf(string[0], "CLEAR: %d", wait2[player]);
	printFont(15 + 24 * player - 12 * maxPlay, 14, string[0], count % 9 * (statc[player * 10 ]==2));
	sprintf(string[0], "LOCK: %d",  wait3[player]);
	printFont(15 + 24 * player - 12 * maxPlay, 15, string[0], count % 9 * (statc[player * 10 ]==3));
	sprintf(string[0], "DAS: %d",  waitt[player]);
	printFont(15 + 24 * player - 12 * maxPlay, 16, string[0], count % 9 * (statc[player * 10 ]==4));


	sprintf(string[0], "SPEED:%4d",  sp[player]);
	printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], count % 9 * (statc[player * 10 ]==5));

	sprintf(string[0], "BGM: %d",  p_bgmlv);
	printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], count % 9 * (statc[player * 10 ]==6));

	if(statc[player * 10] > 0)
		printFont(15 + 24 * player - 12 * maxPlay, 22, "A:START", count % 9);
	else
		printFont(15 + 24 * player - 12 * maxPlay, 22, "A:LOAD", count % 9);

	if(std_opt[player])
	printFont(15 + 24 * player - 12 * maxPlay, 24, "ULTRA2MIN", 9);
	else
	printFont(15 + 24 * player - 12 * maxPlay, 24, "40LINES", 9);

//	printFont(14 + 24 * player - 12 * maxPlay, 9 + statc[player * 10], "b", 1);

	if(backno > bg_max) backno = bg_max;

	// 押しっぱなし移動 #1.60c7n4
	padRepeat(player);
	padRepeat2(player);

	// ↑
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) ){
		if(getPressState(player, 0)) {
			PlaySE(5);
			statc[player * 10]--;
			if(statc[player * 10] < 0) statc[player * 10] = 6;

			statc[player * 10 + 2] = 1;
		}
	}

	// ↓
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) ){
		if(getPressState(player, 1)) {
			PlaySE(5);
			statc[player * 10]++;
			if(statc[player * 10] > 6) statc[player * 10] = 0;

			statc[player * 10 + 2] = 1;
		}
	}
	// ← (Cボタンを押しながらだと高速に数値を変更します)
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
		if(getPressState(player, 2)) {
			PlaySE(3);
					// FAVORITES
			if(statc[player * 10] == 0) {
				statc[player * 10 + 1]--;
				if(statc[player * 10 + 1] < 0) statc[player * 10 + 1] = skip_fwait;
			}

			// 出現
			if(statc[player * 10] == 1) {
				wait1[player]--;
				if(wait1[player] < 0) wait1[player] = 99;
			}
			// 消去
			if(statc[player * 10] == 2) {
				wait2[player]--;
				if(wait2[player] < 0) wait2[player] = 99;
			}
			// 接着
			if(statc[player * 10] == 3) {
				wait3[player]--;
				if(wait3[player] < 0) wait3[player] = 99; // 0以下だったら99に
			}
			// 横溜め
			if(statc[player * 10] == 4) {
				waitt[player]--;
				if(waitt[player] < 0) waitt[player] = 99;
			}
			// 落下スピード
			if(statc[player * 10] == 5) {
				sp[player]--;
				if(sp[player] < 0) sp[player] = 1200; // 0以下だったら1200に
			}
			// BGM
			if(statc[player * 10] == 6) {
				p_bgmlv--;
				if(p_bgmlv < 0) p_bgmlv = 19; // 0以下だったら1200に
			}
		}

		// → (Cボタンを押しながらだと高速に数値を変更します)
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
		if(getPressState(player, 3)) {
			PlaySE(3);
			// FAVORITES
			if(statc[player * 10] == 0) {
				statc[player * 10 + 1]++;

				if(statc[player * 10 + 1] > skip_fwait) statc[player * 10 + 1] = 0;
			}

			// 出現
			if(statc[player * 10] == 1) {
				wait1[player]++;
				if(wait1[player] > 99) wait1[player] = 0;
			}
			// 消去
			if(statc[player * 10] == 2) {
				wait2[player]++;
				if(wait2[player] > 99) wait2[player] = 0;
			}
			// 接着
			if(statc[player * 10] == 3) {
				wait3[player]++;
				if(wait3[player] > 99) wait3[player] = 0;
			}
			// 横溜め
			if(statc[player * 10] == 4) {
				waitt[player]++;
				if(waitt[player] > 99) waitt[player] = 0;
			}
			// 落下スピード
			if(statc[player * 10] == 5) {
				sp[player]++;
				if(sp[player] > 1200) sp[player] = 0;
			}// BGM
			if(statc[player * 10] == 6) {
				p_bgmlv++;
				if(p_bgmlv > 19) p_bgmlv = 0;
			}
		}



	if(getPushState(player, 5)) {		// Bボタンでモード選択画面に戻る
		sp[player] = 1;
		PlaySE(5);
		stat[player] = 1;				// ブロックシャッター実行
		statc[player * 10] = 0;			// ステータスカウンタを0に
		statc[player * 10 + 1] = 2;		// シャッター後はステータスNo.2
		relaymode[player] = 0;
	}

	if((statc[player * 10 + 1] > 15 * 60) || (getPushState(player, 4))) {
		if(statc[player * 10] == 0) {
			// FAVORITES関連
			loadWait(player, statc[player * 10 + 1]); // 対応した設定を読み込む。
			PlaySE(3);
		}else{
		PlaySE(10);
		upLines[player] = 0;
		if(std_opt[player] == 1){			//TI
			ltime[player] = 120 * 60;
		}
		// BGMバグ修正 #1.60c7s6
		bgmlv = p_bgmlv;

		stat[player] = 1;					// ブロックシャッター実行
		statc[player * 10] = 0;				// ステータスカウンタを0に
		statc[player * 10 + 1] = 3;			// シャッター後はステータスNo.3
		statc[player * 10 + 2] = 0;			// ステータスカウンタを0に
		statc[player * 10 + 3] = 0;
		}
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.12 - debug = 1のときのレベルセレクト
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statSelectStartLv(int player) {
	int	i;
	i = 0;

	statc[player * 10 + 1]++;

//	printFont(16 + 24 * player - 12 * maxPlay, 6, "SELECT", 4);
//	printFont(19 + 24 * player - 12 * maxPlay, 7, "LEVEL", 4);
	printSMALLFont(124 + 192 * player - 96 * maxPlay, 45, "Select Level", 4);

	slv[player] = startlvTbl[i + ori_opt[player] * 14];

	for(i = 0; i <= 13; i++) {
		if(gameMode[player]==10) {
			sprintf(string[0], "%4d", startlvTbl[i + ori_opt[player] * 14]);
		} else {
			sprintf(string[0], "%4d", 100 * i);
		}
		printFont(18 + 24 * player - 12 * maxPlay, 9 + i, string[0], count % 9 * (statc[player * 10] == i));
	}
	printFont(17 + 24 * player + (gameMode[player]==10) - 12 * maxPlay, 9 + statc[player * 10], "b", 1);

	sprintf(string[0], "%2d", 15 - (statc[player * 10 + 1] / 60));
	printFont(17 + 24 * player - 12 * maxPlay, 23, "TIME", 6);
	printFont(21 + 24 * player - 12 * maxPlay, 23, string[0], 0);

	// 背景設定
	if((gameMode[player]==10) && (statc[player * 10] <= 10)) backno = statc[player * 10];
	else if(statc[player * 10] > 10) backno = 11;
	else if(gameMode[player] == 0) backno = startlvTbl[statc[player]] / 10;
	else if(gameMode[player] == 3) backno = startlvTbl[statc[player] + 42] /3;
	else backno = startlvTbl[statc[player] + gameMode[player] * 14] /6;

	if(backno > bg_max) backno = bg_max;

	// 押しっぱなし移動 #1.60c7n4
	padRepeat2(player);

	// ↑
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if(getPressState(player, 0)) {
		PlaySE(5);
		statc[player * 10]--;
		if(statc[player * 10] < 0) statc[player * 10] = 13;

		statc[player * 10 + 2] = 1;
	}

	// ↓
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if(getPressState(player, 1)) {
		PlaySE(5);
		statc[player * 10]++;
		if(statc[player * 10] > 13) statc[player * 10] = 0;

		statc[player * 10 + 2] = 1;
	}

	if(statc[player * 10 + 2] || (statc[player * 10 + 1] == 1)) {
		if(gameMode[player]!=10) start[player] = statc[player * 10] * 100;
		else start[player] = startlvTbl[statc[player * 10] + ori_opt[player] * 14];
		setStartLevel(player);
		statc[player * 10 + 2] = 0;
	}


	if(getPushState(player, 5)) {		// Bボタンでモード選択画面に戻る
		PlaySE(5);
		stat[player] = 2;					// ステータスカウンタを0に
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 2] = 0;
	}

	if((statc[player * 10 + 1] > 15 * 60) || (getPushState(player, 4))) {
		PlaySE(10);
		upLines[player] = 0;

		// BGMバグ修正 #1.60c7s6
		bgmlv = setstartBGM_debug(gameMode[player], player);

		startLvback[0] = statc[player * 10];	// 開始レベル記憶 #1.60c7
		stat[player] = 1;					// ブロックシャッター実行
		statc[player * 10] = 0;				// ステータスカウンタを0に
		statc[player * 10 + 1] = 3;			// シャッター後はステータスNo.3
		statc[player * 10 + 2] = 0;			// ステータスカウンタを0に
		statc[player * 10 + 3] = 0;
	}
}


void setStartLevel(int pl) {				// 各種速度設定ロード (loadReplayからも利用) #1.60c3
	if((!fpbas_mode[pl])&&(gameMode[pl]!=10)) { //TGM
		tr[pl] = start[pl] / 10;
		tc[pl] = start[pl];
	} else {       //Heboris
		lv[pl] = start[pl];
		tc[pl] = start[pl] * 10;
	}

	sp[pl] = 1200;
	// 初期値はspeed.defで設定した値を使用する #1.60c7g3
//	wait1[pl] = 26;
//	wait2[pl] = 40;
//	wait3[pl] = 28;
//	waitt[pl] = 15;

	if(repversw < 6) {// このバージョンからせり上がり間隔をレベルではなく、落下させた回数に変更 #1.60c7i2
		// せり上がり開始レベルになってもすぐにせり上がらないようにした #1.60c7g3
		if(gameMode[pl]!=10) shirase[pl] = (tc[pl] / p_shirase_interval + 1) * p_shirase_interval;
    }
	else {
		if(gameMode[pl]!=10) shirase[pl] = raise_shirase_interval;
    }

	// jump(gameMode[pl], lbeg, lmas, l20g, ldvl, lver, lpra, ltom, lace, lmis,lstd,lori);

	switch (gameMode[pl]) {
		case 0: goto lbeg;
		case 1: goto lmas;
		case 2: goto l20g;
		case 3: goto ldvl;
		case 4: goto lver;
		case 5: goto lpra;
		case 6: goto ltom;
		case 7: goto lace;
		case 8: goto lmis;
		case 9: goto lstd;
		case 10:goto lori;
	}

	lbeg:	// Beginner
		if(start[pl] < 1000){
			if(repversw < 40)sp[pl] = lvTableBeg39[tr[pl]];
			else sp[pl] = lvTableBeg[tr[pl]];
		}
		wait1[pl] = wait1_Beginner;
		wait2[pl] = wait2_Beginner;
		wait3[pl] = wait3_Beginner;
		waitt[pl] = waitt_Beginner;

		timelimit[pl] = 0;		// 足きりタイム#1.60c7g7
		goto next;
	lmas:	// Master
		if(tr[pl] < 50) {
			sp[pl] = lvTableTgm[tr[pl]];
			wait1[pl] = wait1_master_half;
			wait2[pl] = wait2_master_half;
			wait3[pl] = wait3_master_half;
			waitt[pl] = waitt_master_half;
		} else {
			sp[pl] = 1200;
			if(repversw >= 54){
				wait1[pl] = wait1_master_tbl[tr[pl] / 10];
				wait2[pl] = wait2_master_tbl[tr[pl] / 10];
				wait3[pl] = wait3_master_tbl[tr[pl] / 10];
				waitt[pl] = waitt_master_tbl[tr[pl] / 10];
			}else if(repversw >= 29){
				wait1[pl] = wait1_master_tbl53[tr[pl] / 10];
				wait2[pl] = wait2_master_tbl53[tr[pl] / 10];
				wait3[pl] = wait3_master_tbl53[tr[pl] / 10];
				waitt[pl] = waitt_master_tbl53[tr[pl] / 10];
			}else{
				wait1[pl] = wait1_master_tbl28[(tr[pl] - 50) / 10 * 2];
				wait2[pl] = wait2_master_tbl28[(tr[pl] - 50) / 10 * 2];
				wait3[pl] = wait3_master_tbl28[(tr[pl] - 50) / 10 * 2];
				waitt[pl] = waitt_master_tbl28[(tr[pl] - 50) / 10 * 2];
			}
		}
		if(!playback) timelimit[pl] = timelimit_master;	// 足きりタイム(master)#1.60c7g7
		if(enable_grade[pl]== 4)tr2[pl] = tr[pl];
		goto next;
	l20g:	// 20G
		if(tr[pl] < 50) {
			wait1[pl] = wait1_20G_half;
			wait2[pl] = wait2_20G_half;
			wait3[pl] = wait3_20G_half;
			waitt[pl] = waitt_20G_half;
		} else {
			sp[pl] = 1200;
			if(repversw >= 54){
				wait1[pl] = wait1_master_tbl[tr[pl] / 10];
				wait2[pl] = wait2_master_tbl[tr[pl] / 10];
				wait3[pl] = wait3_master_tbl[tr[pl] / 10];
				waitt[pl] = waitt_master_tbl[tr[pl] / 10];
			}else if(repversw >= 29){
				wait1[pl] = wait1_master_tbl53[tr[pl] / 10];
				wait2[pl] = wait2_master_tbl53[tr[pl] / 10];
				wait3[pl] = wait3_master_tbl53[tr[pl] / 10];
				waitt[pl] = waitt_master_tbl53[tr[pl] / 10];
			}else{
				wait1[pl] = wait1_master_tbl28[(tr[pl] - 50) / 10 * 2];
				wait2[pl] = wait2_master_tbl28[(tr[pl] - 50) / 10 * 2];
				wait3[pl] = wait3_master_tbl28[(tr[pl] - 50) / 10 * 2];
				waitt[pl] = waitt_master_tbl28[(tr[pl] - 50) / 10 * 2];
			}
		}
		if(!playback) timelimit[pl] = timelimit_20G;		// 足きりタイム(20G)#1.60c7g7
		if(enable_grade[pl]== 4)tr2[pl] = tr[pl];
		goto next;
	ldvl:	// Devil
		tr[pl] = start[pl] / 10;
		if(repversw < 18){
			wait1[pl] = wait1_devil_tbl17[tr[pl] / 10];
			wait2[pl] = wait2_devil_tbl17[tr[pl] / 10];
			wait3[pl] = wait3_devil_tbl17[tr[pl] / 10];
			waitt[pl] = waitt_devil_tbl17[tr[pl] / 10];
		}
		if(repversw == 18){
			wait1[pl] = wait1_devil_tbl18[tr[pl] / 10];
			wait2[pl] = wait2_devil_tbl18[tr[pl] / 10];
			wait3[pl] = wait3_devil_tbl18[tr[pl] / 10];
			waitt[pl] = waitt_devil_tbl18[tr[pl] / 10];
		}
		if(repversw >= 19){
			if((((enable_grade[pl] == 1) && (repversw < 42)) || (devil_minus[pl]))&&(gameMode[pl] == 3)&&(repversw >= 31)){//devil-
				wait1[pl] = wait1_devil_m_tbl[tr[pl] / 10];
				wait2[pl] = wait2_devil_m_tbl[tr[pl] / 10];
				wait3[pl] = wait3_devil_m_tbl[tr[pl] / 10];
				waitt[pl] = waitt_devil_m_tbl[tr[pl] / 10];
			}else if(repversw < 33){
				wait1[pl] = wait1_devil_tbl32[tr[pl] / 10];
				wait2[pl] = wait2_devil_tbl32[tr[pl] / 10];
				wait3[pl] = wait3_devil_tbl32[tr[pl] / 10];
				waitt[pl] = waitt_devil_tbl32[tr[pl] / 10];

			}else if(repversw < 60){
				wait1[pl] = wait1_devil_tbl[tr[pl] / 10];
				wait2[pl] = wait2_devil_tbl[tr[pl] / 10];
				wait3[pl] = wait3_devil_tbl[tr[pl] / 10];
				waitt[pl] = waitt_devil_tbl[tr[pl] / 10];
			}else if(repversw < 62){
				wait1[pl] = wait1_doom_tbl61[tr[pl] / 10];
				wait2[pl] = wait2_doom_tbl61[tr[pl] / 10];
				wait3[pl] = wait3_doom_tbl61[tr[pl] / 10];
				waitt[pl] = waitt_doom_tbl61[tr[pl] / 10];
			}else{//普通
				wait1[pl] = wait1_doom_tbl[tr[pl] / 10];
				wait2[pl] = wait2_doom_tbl[tr[pl] / 10];
				wait3[pl] = wait3_doom_tbl[tr[pl] / 10];
				waitt[pl] = waitt_doom_tbl[tr[pl] / 10];
			}
		}
		if(!playback) {
			//DOOMの足切りタイムを回転法則ごとに変更
			if((rots[pl]==3) || (rots[pl]==4) || (rots[pl]==6))
				timelimit[pl] = timelimit_doom_E;
			else if((rots[pl]==0) || (rots[pl]==2) || (rots[pl]==5))
				timelimit[pl] = timelimit_doom_N;
			else
				timelimit[pl] = timelimit_doom_H;

			timelimitw[pl] = timelimit[pl];

			timelimitm[pl] = timelimitm_devil;
			timelimitmw[pl] = timelimitmw_devil;
			//1000の足切りは500の2倍
			timelimit2[pl] = timelimit[pl] * 2;	// #1.60c7q8
		}

		goto next;
	lver:	// Versus
		sp[pl] = speed_versus;
		wait1[pl] = wait1_versus;
		wait2[pl] = wait2_versus;
		wait3[pl] = wait3_versus;
		waitt[pl] = waitt_versus;
		goto next;
	lpra:	// Practice
		goto next;
	ltom:	// Tomoyo #1.60c7m1
		if(!fpbas_mode[pl]) {
			if(start[pl] < 300) sp[pl] = lvTabletomoyo[tr[pl]];
			if(repversw < 27){
				wait1[pl] = wait1_tomoyo26;
				wait2[pl] = wait2_tomoyo26;
				wait3[pl] = wait3_tomoyo26;
				waitt[pl] = waitt_tomoyo26;
			}else{
				wait1[pl] = wait1_tomoyo;
				wait2[pl] = wait2_tomoyo;
				wait3[pl] = wait3_tomoyo;
				waitt[pl] = waitt_tomoyo;
			}
			timelimit[pl] = 0;
		}else {
			sp[pl] = lvTabletomoyohebo[lv[pl]];
			wait1[pl] = wait1_tomoyo;
			wait2[pl] = wait2_tomoyo;
			wait3[pl] = wait3_tomoyo;
			waitt[pl] = waitt_tomoyo;
		}
		goto next;
	lace:	// Ace
		if(anothermode[pl] == 1){//初期アナザー読み込み
			sp[pl] = lvTableAcea[0];
			wait1[pl] = wait1_Acea_tbl[0];
			wait2[pl] = wait2_Acea_tbl[0];
			wait3[pl] = wait3_Acea_tbl[0];
			waitt[pl] = waitt_Acea_tbl[0];
			goto next;

		}else if(anothermode[pl] == 2){//初期アナザー2読み込み
			sp[pl] = lvTableAcea[0];
			wait1[pl] = wait1_Acea_tbl[14];
			wait2[pl] = wait2_Acea_tbl[14];
			wait3[pl] = wait3_Acea_tbl[14];
			waitt[pl] = waitt_Acea_tbl[14];
			isblind[pl] = 1;
			goto next;
		}else if(anothermode[pl] == 3){//初期アナザー3読み込み
			if((isWRule(pl)) && (repversw >= 54)){
				heboGB[pl] = 1;//ワールド系
				sp[pl] = lvTableHeboGB[0];
				wait1[pl] = wait1_HeboGB;
				wait2[pl] = wait2_HeboGB;
				wait3[pl] = wait3_HeboGB_tbl[0];
				waitt[pl] = waitt_HeboGB_tbl[0];
			}else{
				heboGB[pl] = 2;//SEGA系
				sp[pl] = lvTabletomoyohebo[0];
				wait1[pl] = 30;
				wait2[pl] = 39;
				wait3[pl] = 30;
				waitt[pl] = 20;
			}
			goto next;

		}else{//ノーマル読み込み
			if(repversw < 56)
				sp[pl] = lvTableAce55[0];
			else
				sp[pl] = lvTableAce[0];
			wait1[pl] = wait1_Ace;
			wait2[pl] = wait2_Ace;
			wait3[pl] = wait3_Ace;
			waitt[pl] = waitt_Ace;
			goto next;
		}
	lmis:	// Mission(リプレイのみ)
		goto next;
	lstd:	// Next
		if(std_opt[pl] == 2){//SQUARE読み込み
			if(repversw < 56)
				sp[pl] = lvTableAce55[0];
			else
				sp[pl] = lvTableAce[0];
			squaremode[pl] = 1;
			goto next;

		}else if(std_opt[pl] == 3){//SQUARE読み込み
			if(repversw < 56)
				sp[pl] = lvTableAce55[0];
			else
				sp[pl] = lvTableAce[0];
			goto next;

		}
		goto next;
	lori:	//ORIGINAL
		if(ori_opt[pl]==0){
			if(start[pl] < 100){
			sp[pl] = lvTableBeg39[lv[pl]];
			}
			wait1[pl] = wait1_Beginner;
			wait2[pl] = wait2_Beginner;
			wait3[pl] = wait3_Beginner;
			waitt[pl] = waitt_Beginner;
		}else if((ori_opt[pl]==1)||((ori_opt[pl]==2))){
			if(lv[pl] < 30) {
				if(ori_opt[pl]==1){
				sp[pl] = orimasterlvTable[lv[pl]];
				}else if(ori_opt[pl]==2){
				sp[pl] = 1200;
				}
				wait1[pl] = wait1_master_half;
				wait2[pl] = wait2_master_half;
				wait3[pl] = wait3_master_half;
				waitt[pl] = waitt_master_half;
			} else {
				sp[pl] = 1200;
				wait1[pl] = wait1_orimaster_tbl[(lv[pl] - 30) / 3];
				wait2[pl] = wait2_orimaster_tbl[(lv[pl] - 30) / 3];
				wait3[pl] = wait3_orimaster_tbl[(lv[pl] - 30) / 3];
				waitt[pl] = waitt_orimaster_tbl[(lv[pl] - 30) / 3];
			}
		}else if(ori_opt[pl]==3){
			sp[pl] = 1200;
			wait1[pl] = wait1_oridevil_tbl[lv[pl] / 3];
			wait2[pl] = wait2_oridevil_tbl[lv[pl] / 3];
			wait3[pl] = wait3_oridevil_tbl[lv[pl] / 3];
			waitt[pl] = waitt_oridevil_tbl[lv[pl] / 3];
		}
	next:	;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.03 - Ready !?
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statReady(int player) {
//	int r_start, r_end, g_start, g_end;
	int tmp;

	/* コンピュータ操作 #1.60c7o3 */
	if(playback) {
		cpu_flag[player] = 0;	// リプレイ時はCPU解除 #1.60c7o6
	} else {
		if( (!player && cp_player_1p) || (player && cp_player_2p) ) cpu_flag[player] = 1;
	}
	if((!playback) || (repversw < 37)){
		if(ready_go_style == 0){	// HEBORIS式
			if( ((gameMode[player] == 6) && (stage[player] < 100)) || ((gameMode[player] == 5) && (p_nextpass)) ) {
				/* TOMOYO #1.60c7l7 */
				// PRACTICEでNEXT PASSを使った場合も長くする #1.60c7o8
				r_start[player] = s_ready_start;
				r_end[player]   = s_ready_end;
				g_start[player] = s_go_start;
				g_end[player]   = s_go_end;
			} else {
				/* 通常 */
				r_start[player] = ready_start;
				r_end[player]   = ready_end;
				g_start[player] = go_start;
				g_end[player]   = go_end;
			}
		} else {					// LITE版風
			if( ((gameMode[player] == 6) && (stage[player] < 100)) || ((gameMode[player] == 5) && (p_nextpass)) ) {
				/* TOMOYO #1.60c7l7 */
				// PRACTICEでNEXT PASSを使った場合も長くする #1.60c7o8
				r_start[player] = ti_s_ready_start;
				r_end[player]   = ti_s_ready_end;
				g_start[player] = ti_s_go_start;
				g_end[player]   = ti_s_go_end;
			} else {
				/* 通常 */
				r_start[player] = ti_ready_start;
				r_end[player]   = ti_ready_end;
				g_start[player] = ti_go_start;
				g_end[player]   = ti_go_end;
			}
		}
	}
	// bigtypeが2の場合はモードによって移動単位を決める #1.60c7o9
	if( (bigtype == 2) && (!playback) ) {
		if( (gameMode[player] == 4) || (gameMode[player] == 6) || ((item_mode[player]) && (IsBigStart[player] != 1))) {
			BigMove[player] = 0;
		} else {
			BigMove[player] = 1;
		}
	}

	padRepeat(player);

	// DEVILのランダムせり上がり C7U4.9
	if((statc[player * 10] == 0) && (gameMode[player] == 3) && (devil_randrise) && (repversw >= 44)){
		if((devil_rise_min[tr[player] / 10] > 0) && (devil_rise_max[tr[player] / 10] > 0) && (!devil_minus[player])) {
			do {
				devil_nextrise[player] = rand( devil_rise_max[tr[player] / 10] + 1,player);
			} while(devil_nextrise[player] < devil_rise_min[tr[player] / 10]);
		}
	}

	// 最初のミッション
	if((statc[player * 10] == 0) && (gameMode[player] == 8) && (c_mission == start_mission) && (c_norm[player] == 0) && (repversw >= 54)){
		missionSetStatus();
	}

	// tomoyoモードでステージを読み込む #1.60c7l6
	if( (statc[player * 10] == 0) && (gameMode[player] == 6) ) {
		loadTomoyoStage(player, stage[player]);
		if( (repversw >= 58) && ((stage[player] >= 45) && (stage[player] <= 71)) )
			randPlatinum(player, ((stage[player] - 44) / 5) + 2);
		if(stage[player] >= 100){
			if(repversw >= 47) FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
			else FP_bonus[player] = 10800;
		}
		if(wavebgm >= 1) {
			if( !IsPlayWave(50 +bgmlv) ) PlayWave(50 +bgmlv);
		}

		if((repversw >= 25) && (sp[player] < min_speed[player]) && (enable_minspeed) || (tomoyo_waits[player]))
			sp[player] = min_speed[player];
		if((stage[player] == 44) && (!disable_ehlast_gimmick)){
			tomoyo_domirror[player] = 1;
			if(!tmp_maxPlay){
				maxPlay = 1;
				backno = 61;
			}
		}
		if(tomoyo_waits[player]){
			if(enable_minspeed){
				if(repversw >= 50) sp[player] = 1200;
				wait1[player] = wait1_devil_tbl32[5];
				wait2[player] = wait2_devil_tbl32[5];
				wait3[player] = wait3_devil_tbl32[5];
				waitt[player] = waitt_devil_tbl32[5];
			}else if(repversw >= 54){
				isboost[player] = 1;
				item_timer[player] = 1500 * 60;
			}
		}
		rest_pblock[player] = getPlatinumBlock(player);
		onRecord[player] = 1;				// リプレイ記録開始#1.60c7n8
		max_hnext[player] = hnext[player];

		if(rest_pblock[player] == 0) {
			// プラチナブロックが1個もない場合は即エンド #1.60c7r7
			PlaySE(28);
			stat[player] = 13;
			ending[player] = 1;
			return;
		}
		if( isfmirror[player] ){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 1, 0);
		}else if( rollroll_flag[player] ){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 2, 0);
		}else if( xray_flag[player] ){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 4, 0);
		}else if( color_flag[player] ){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 5, 0);
		}else if( IsBig[player] ){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 3, 0);
		}else if( tomoyo_waits[player] ){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 12, 0);
		}else if( hnext_flag[player]){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 7, 0);
		}else if((blind_flag[player]) || (isthunder[player]) || (tomoyo_rise[player])){
			PlaySE(38);
			objectCreate2(player, 10, 0, 0, 0, 0, 0, 0);
		}
	}

	if((gameMode[player] == 6) && (statc[player * 10] >= r_start[player])){	// STAGEの表示
		printFont(17 + 24 * player - 12 * maxPlay, 9, "STAGE", fontc[rots[player]]);
		if( (stage[player] >= 20) && (stage[player] <= 26) ) {
			sprintf(string[0],"  EX%2d",stage[player] - 19);
		} else if( (stage[player] >= 27) && (stage[player] <= 44) ){
			sprintf(string[0],"E-HEART%2d",stage[player] - 26);
		} else if( (stage[player] >= 45) && (stage[player] <= 71) ){
			sprintf(string[0],"ACE-TGT%2d",stage[player] - 44);
		} else if(stage[player] >= 100){
			sprintf(string[0],"F-POINT%2d",stage[player] - 99);
		} else {
			sprintf(string[0],"   %d",stage[player] + 1);
		}
		printFont(16 + 24 * player - 12 * maxPlay, 10, string[0],  0 );
	}

	if((statc[player * 10] == 0) && ((ace_irs != 0) && (gameMode[player] != 6) || (gameMode[player] == 4))){
		onRecord[player] = 1;				// ACE式IRSなら、すぐリプレイ記録開始
		max_hnext[player] = hnext[player];
	}

	// MISSIONモードのターゲット
	if( (statc[player * 10] == 0) && (gameMode[player] == 8) && (mission_type[c_mission] == 6) && (c_norm[player] == 0) ) {
		loadTargetStage();
	}

	// TOMOYO用スタート時のキーリピート記録＆再生 #1.60c7n8
	if((statc[player * 10] == 0) || (repversw < 57)){
	if( (gameMode[player] == 6) && (stage[player] == start_stage[player]) || (ace_irs != 0) || ((gameMode[player] == 4) && (vs_round == 1))) {
		if(playback) {
			mp [player] = mps[player * 2    ];
			mpc[player] = mps[player * 2 + 1];
		} else {
			mps[player * 2    ] = mp [player];
			mps[player * 2 + 1] = mpc[player];
		}
	}
	}

	if(( gameMode[player] == 7 ) && (anothermode[player] == 0)) {
		// ACEモードの制限時間設定
		ltime[player] = timeLimitAce[0];
		lv[player] = 1;
	}

	if(( gameMode[player] == 7 ) && (anothermode[player] == 1)) {
		// ACEアナザーモードの制限時間設定
		ltime[player] = timeLimitAcea[0];
		lv[player] = 1;
	}

	if(( gameMode[player] == 7 ) && (anothermode[player] == 2)) {
		// ACEアナザー2モードの制限時間設定
		ltime[player] = timeLimitAcea2[0];
		lv[player] = 1;
	}
	if(( gameMode[player] == 7 ) && (anothermode[player] == 3)) {
		// ACEアナザー2モードの制限時間設定
		//ltime[player] = timeLimitAcea2[0];
		lv[player] = 0;
	}
	if(( gameMode[player] == 9 ) && (std_opt[player] >= 2)) {
		lv[player] = 1;
	}

	// READY GOのタイミングを設定できるようにした #1.60c7j5
	if(ready_go_style == 0){	// HEBORIS
		if(statc[player * 10] < r_start[player]) {

		} else if((statc[player * 10] < r_end[player]) && (r_end[player] > r_start[player])) {
			ExBltRect(3, 121 + player * 192 - 96 * maxPlay, 239 - ((statc[player * 10] - r_start[player]) * (1500 / (r_end[player] - r_start[player]))) / 100 * 8, 0, 0, 78, 17);
		} else if((statc[player * 10] < g_start[player]) && (g_start[player] >= r_end[player])) {
			ExBltRect(3, 121 + player * 192 - 96 * maxPlay, 119, 0, 0, 78, 17);
		} else if(g_end[player] > g_start[player]) {
			ExBltRect(3, 121 + player * 192 - 96 * maxPlay, 119 - ((statc[player * 10] - g_start[player]) * (1500 / (g_end[player] - g_start[player]))) / 100 * 8, 0, 0, 78, 17);
			ExBltRectR(3, 117 + player * 192 - ((g_end[player] - statc[player * 10]) * (1500 / (g_end[player] - g_start[player]))) / 100 * 86 / 16 - 96 * maxPlay,
				119 - ((g_end[player] - statc[player * 10]) * (1500 / (g_end[player] - g_start[player]))) / 100 * 17 / 16, 0, 17, 86, 17,
				65536 + ((g_end[player] - statc[player * 10]) * (1500 / (g_end[player] - g_start[player]))) / 100 * 8192,
				65536 + ((g_end[player] - statc[player * 10]) * (1500 / (g_end[player] - g_start[player]))) / 100 * 8192);
		}
	} else {					// LITE
		if(statc[player * 10] < r_start[player]) {
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 98  - (r_start[player] - statc[player * 10]),  (statc[player * 10] % 40) * 3, 28, 80, 8);
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 126 + (r_start[player] - statc[player * 10]),  160 - ((statc[player * 10] % 40) * 3), 20, 80, 8);
		} else if( (statc[player * 10] >= r_start[player]) && (statc[player * 10] < r_end[player]) ) {
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 98  ,  (statc[player * 10] % 40) * 3, 28, 80, 8);
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 126 ,  160 - ((statc[player * 10] % 40) * 3), 20, 80, 8);
			// READY
			ExBltRect(81, 110+192 * player -96 * maxPlay , 91 , 200, 0, 100, 50);
		} else if( (statc[player * 10] >= r_end[player]) && (statc[player * 10] < g_start[player]) ) {
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 98  ,  (statc[player * 10] % 40) * 3, 28, 80, 8);
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 126 ,  160 - ((statc[player * 10] % 40) * 3), 20, 80, 8);
		} else if( (statc[player * 10] >= g_start[player]) && (statc[player * 10] < g_end[player]) ) {
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 98  - (statc[player * 10] - g_start[player]),  (statc[player * 10] % 40) * 3, 28, 80, 8);
			ExBltRect(77, 120 + 192 * player -96 * maxPlay , 126 + (statc[player * 10] - g_start[player]),  160 - ((statc[player * 10] % 40) * 3), 20, 80, 8);
			// GO
			ExBltRect(81, 110+192 * player -96 * maxPlay , 91 , 200, 50, 100, 50);
		}
	}

	if(statc[player * 10] == r_start[player]) PlaySE(15);
	if(statc[player * 10] == g_start[player]) PlaySE(16);
	if(statc[player * 10] >= g_end[player]) {
		// PRACTICEでエンディング中の場合#1.60c6.2d
		if((gameMode[0] == 5) && (ending[0] != 0)) {
			if(ending[0] == 1){
				PlayWave(56);						// BGM流れてなかったorz 差し替え#1.60c7i4
				stat[player] = 13;					// エンディング開始
				statc[player * 10] = 0;				// あとかたづけ
				// エンディング突入を高速化#1.60c7i4
				ending[player] = 2;
				edrec[player] = 125;
				timeOn[player] = 0;					// タイマーストップ
				onRecord[player] = 1;				// リプレイ記録開始
			}else if(ending[0] == 6){
				stat[player] = 13;					// エンディング開始
				statc[player * 10] = 0;				// あとかたづけ
				timeOn[player] = 0;					// タイマーストップ
				onRecord[player] = 1;				// リプレイ記録開始
			}
		} else {
			if(wavebgm >= 1) {
				if( !IsPlayWave(50 +bgmlv) ) PlayWave(50 +bgmlv);
			}

			stat[player] = 4;
			statc[player * 10 + 1] = 1;				// 1..Reday!
			statc[player * 10] = 0;					// あとかたづけ
			timeOn[player] = 1;						// タイマー開始
			onRecord[player] = 1;					// リプレイ記録開始
			max_hnext[player] = hnext[player];
			if(color_flag[player]){
				iscolor[player] = 1;				// COLOR#1.60c7p9ex
			}
			if((gameMode[player] != 6) && (gameMode[player] != 8) && (!ace_irs) && (gameMode[player] != 4)) {
				// 通常モード用スタート時のキーリピート記録＆再生
				if(playback) {
					mp [player] = mps[player * 2    ];
					mpc[player] = mps[player * 2 + 1];
				} else {
					mps[player * 2    ] = mp [player];
					mps[player * 2 + 1] = mpc[player];
				}
			}
			if((gameMode[player] == 8) && (!ace_irs) && (c_mission == start_mission) && (c_norm[player] == 0) && (gameMode[player] != 4)) {
				// ミッションモード用スタート時のキーリピート記録＆再生
				if(playback) {
					mp [player] = mps[player * 2    ];
					mpc[player] = mps[player * 2 + 1];
				} else {
					mps[player * 2    ] = mp [player];
					mps[player * 2 + 1] = mpc[player];
				}
			}
		}
		pass_flg[player] = 0;		// #1.60c7j5
	} else {
		if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5
		if(gameMode[player] == 4){	//VSのスタイル
			sprintf(string[0]," Round %d",vs_round);
			printSMALLFont(130 + 192 * player - 96 * maxPlay, 70, string[0], 0);
			if(!disrise){
			if(statc[player * 10] < g_start[player]){
				printSMALLFont(124 + 192 * player - 96 * maxPlay, 127, "Select Style", count % 9);
				ExBltRect(25,120 + 192 * player - 96 * maxPlay, 136, 240, 168, 80, 56);
				if(noitem)	//ITEMに×マーク
					ExBltRect(3,127 + 192 * player - 96 * maxPlay, 166, 274, 384, 12, 12);
				if(getPushState(player, 1)){
					PlaySE(5);
					vs_style[player] = 0;	//NORMAL
				}else if(getPushState(player, 0)){
					PlaySE(5);
					vs_style[player] = 1;	//ATTACK
				}else if(getPushState(player, 3)){
					PlaySE(5);
					vs_style[player] = 2;	//DEFENCE
				}else if((getPushState(player, 2)) && (!noitem)){
					PlaySE(5);
					vs_style[player] = 3;	//ITEM
				}
			}else if(statc[player * 10] == g_start[player]){
				if(vs_style[player] == -1){	//入力がなかったらランダム
					PlaySE(5);
					do{
						vs_style[player] = rand(4,player);
					}while((vs_style[player] == 3) && (noitem));
				}
				if(vs_style[player] == 1){	//ATTACKはアイテムゲージ2倍
					item_inter[player] = item_interval * 2;
				}else if(vs_style[player] == 3){	//ITEMはゲージ半分
					item_inter[player] = item_interval / 2;
				}else{	//他は変更しない
					item_inter[player] = item_interval;
				}
			}else if(statc[player * 10] >= g_start[player]){
				if(vs_style[player] == 0){
					printSMALLFont(143 + 192 * player - 96 * maxPlay, 140, "NORMAL", (count % 4 / 2) * digitc[rots[player]]);
				}else if(vs_style[player] == 1){
					printSMALLFont(143 + 192 * player - 96 * maxPlay, 140, "ATTACK", (count % 4 / 2) * digitc[rots[player]]);
				}else if(vs_style[player] == 2){
					printSMALLFont(140 + 192 * player - 96 * maxPlay, 140, "DEFENCE", (count % 4 / 2) * digitc[rots[player]]);
				}else if(vs_style[player] == 3){
					printSMALLFont(143 + 192 * player - 96 * maxPlay, 140, " ITEM ", (count % 4 / 2) * digitc[rots[player]]);
				}
			}
			}
		}
		// ツモ送り機能 #1.60c7j5
		if( (gameMode[player] == 6) || ((gameMode[0] == 5) && (p_nextpass)) ) {
			if((!pass_flg[player]) && (getPressState(player, 7))) {
				PlaySE(6);	// hold.wav

				hold[player] = next[player];

				// HOLDミノの色を設定 #1.60c7p1
				if(p_monochrome_mode==1) {
					c_hblk[player] = (p_monochrome_mode==2) * 8;
				} else {
					if( isWRule(player) )
						c_hblk[player] = wcol[next[player]];
					else if( (rots[player] >= 4) && (rots[player] != 8) )
						c_hblk[player] = acol[next[player]];
					else
						c_hblk[player] = ccol[next[player]];
				}

				nextc[player] = (nextc[player] + 1) % 1400;
				next[player] = nextb[nextc[player] + player * 1400];
				rt_nblk[0 + 6 * player] = 0;
				// ブロックの色を設定 #1.60c7m9
				setNextBlockColors(player,0);
				dhold[player] = 0;
				dhold2[player] = 0;
			}
			pass_flg[player] = getPressState(player, 7);
		}
	}
	statc[player * 10]++;	// ブロック落下開始へ

	// 下入れ制限初期化#1.60c7f9
	down_flag[player] = 0;
	up_flag[player] = 0;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.04 - ブロック落下開始
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statBlock(int player) {
	int i, j,k, m[2],tmp[2],tmp2[2];

	if(dolaser[player]){	//レーザー発動
		stat[player] = 26;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		dolaser[player] = 0;
		return;
	}
	if(donega[player]){	//ネガフィールド発動
		stat[player] = 27;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		donega[player] = 0;
		return;
	}
	if(doshotgun[player]){	//ショットガン発動
		stat[player] = 28;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		doshotgun[player] = 0;
		return;
	}
	if(dorulet[player]){	//アイテムルーレット発動
		stat[player] = 31;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		dorulet[player] = 0;
		return;
	}
	if(do180field[player]){	//180°フィールド発動
		stat[player] = 34;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		do180field[player] = 0;
		return;
	}
	if(doexchg[player]){	//フィールド交換発動
		stat[player] = 29;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		doexchg[player] = 0;
		return;
	}
	if(do16t[player]){	//16t発動
		stat[player] = 26;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 1;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		do16t[player] = 0;
		return;
	}
	if(docopyfld[player]){	//フィールドコピー発動
		stat[player] = 29;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 1;
		statc[player * 10 + 4] = 0;
		docopyfld[player] = 0;
		return;
	}
	harddrop[player]=0;
	// tgmカウンタ上昇(移転)#1.60c7i8
	if((ending[player] == 0) && (gameMode[player] <= 6) && ((!ismiss[player]) || (repversw < 62))) {
		if((repversw > 4) && (gameMode[player] >= 3))
			i = 99;//99でストップ
		else
			i = 99 - ((tc[player] > 900));//マスター、20Gの998ストップ用

		if((gameMode[player] == 4) && (repversw >= 61) && (wintype==0) && (vs_goal != 0)){
			if((tc[player] != vs_goal - 1) && ((!start_flag[player]) || (tc[player] < 0))){
				tc[player]++;
				if(tc[player] == vs_goal - 1) PlaySE(9);
			}
		}else if((gameMode[player] == 0) && (novice_mode[player])){
			if((tc[player]  != 299) && ((!start_flag[player]) || (tc[player] < 0))) {//noviceは299でのみストップ
				tc[player]++;
				if(tc[player] == 299) PlaySE(9);
			}
		}else {
			if((tc[player] % 100 != i) && ((!start_flag[player]) || (tc[player] < 0))) {
				tc[player]++;
				if(tc[player] % 100 == i) PlaySE(9);
			}
		}

		if(start_flag[player]) start_flag[player] = 0;

		// VSでゴールがライン制の場合
		if((gameMode[player] == 4) && (wintype == 1))
			tc[player] = 0;

		if((gameMode[player]!=6)||(tomoyo_opt[player]!=4)||(!fpbas_mode[player])){
			lv[player] = tc[player];
		}
	}

	// TOMOYOせり上がり
	if(((gameMode[player] == 6) || ((gameMode[player] == 8) && (mission_type[c_mission] == 6))) && (tomoyo_rise[player] != 0)){
		tomoyo_rise_t[player]++;
		if(tomoyo_rise_t[player] % tomoyo_rise[player] == 0)
			upLines[player] = 1;
	}

	//HEBO+せり上がり
	if(hebo_plus[player]){
		if(st_bdowncnt[player] >= hebop_interval[player]){
			upLines[player] = 1;
			st_bdowncnt[player] = 0;	// ブロックを着床させた回数 #1.60c7i2
		}
	}
	// ACEモードのタイマー再開
	if(( (gameMode[player] == 7) || (gameMode[player] == 8) ) && (!ending[player])) {
		timeOn[player] = 1;
	}

	if((gameMode[player] == 4) || (item_mode[player])){
		// アイテムゲージ上昇 #1.60c7n1
		if( (!noitem) && ( (!ismiss[player]) || ((gameMode[player] != 4) && (repversw >= 62)) ) ) item_g[player]++;

		// 効果時間
		if(IsBig[player] || isfmirror[player] || isrollroll[player] || isxray[player] || iscolor[player] ||
		isholdlock[player] || ismagnetblock[player] || isremote[player] || istrance[player]||item_monochrome[player]){
			item_t[player]++;
		}
		if(ismiss[player])
			misstimer[player]--;
	}
	for(i = 0; i < 10; i++) statc[i + player * 10] = 0;

	statc[player * 10 + 4] = (fastlrmove == 0);		// 横方向の先行移動

	if(rots[player] == 8) statc[player * 10 + 4] = 0;

	// ドロップボーナスが累積されるバグの修正のため、statEraseBlockから移動 #1.60c7j5
	sdrop[player] = 0;
	qdrop[player] = 0;

	hiddenProc(player);

	padRepeat(player);

	// ↓を離している場合、下入れ制限解除#1.60c7n6
	if((!getPressState(player, 1-(1*((isUDreverse[player]) && (repversw>=44)) ))) && (repversw >= 10)) down_flag[player] = 0;
	if(!getPressState(player, 0+(1*((isUDreverse[player]) && (repversw>=44)) ))) up_flag[player] = 0;

	// T-SPIN権利初期化 #1.60c7n6
	tspin_flag[player] = 0;

	blk[player] = next[player];
	dhold[player] = disable_hold;		// hold使用可能に	#1.60c
	dhold2[player] = 0;

	nextc[player] = (nextc[player] + 1) % 1400;
	if(!isfever[player])
		next[player] = nextb[nextc[player] + player * 1400];
	else		//フィーバー中は棒のみ出現
		next[player] = 0;

	// アイテム出現 #1.60c7n1
	item_coming[player] = 0;
	if( ((gameMode[player] == 4) || (item_mode[player])) && (item_g[player] > item_inter[player]) ) {
		PlaySE(9);
		itemappear_c[player]=30;
		if(use_item[player] == 0){//ALL
			if(repversw >=51){
				do{
					tmp2[player] = rand(item_pronum,player);
					tmp[player] = 1;
					do{
						tmp2[player] = tmp2[player]-item_pro[tmp[player]-1 ];
						if(tmp2[player] < 0){
							break;
						}
							tmp[player]++;
						if(tmp[player]>item_num+1)break;
					}while(1);
				//履歴の中にあったら引き直し
				}while(
					(itemhistory[0+player*5] == tmp[player]) ||
					(itemhistory[1+player*5] == tmp[player]) ||
					(itemhistory[2+player*5] == tmp[player]) ||
					(itemhistory[3+player*5] == tmp[player]) ||
					(itemhistory[4+player*5] == tmp[player])
				);
				item_coming[player] = tmp[player];
				itemhistory[0+player*5]=itemhistory[1+player*5];//履歴をずらす
				itemhistory[1+player*5]=itemhistory[2+player*5];
				itemhistory[2+player*5]=itemhistory[3+player*5];
				itemhistory[3+player*5]=itemhistory[4+player*5];
				itemhistory[4+player*5]=tmp[player];
			}else{
				item_coming[player] = rand(item_num,player);
			}
		}else if((use_item[player] > 0)&&(use_item[player] <= item_num)){//単発
			item_coming[player] = use_item[player];
		}else if(use_item[player] == item_num+1){//few　1~5
			do{
				tmp[player] = rand(item_num,player) + 1;
			}while((itemhistory[0+player*5]==tmp[player])||(6<=tmp[player]));
			item_coming[player] = tmp[player];
			itemhistory[0+player*5]=tmp[player];
		}else if(use_item[player] == item_num+2){//DS　6 7 12 13 18 26
			do{
				tmp[player] = rand(item_num,player) + 1;
				if(tmp[player] == 6){//サンダーのときはもう一回
					tmp[player] = rand(item_num,player) + 1;
				}
			}while((itemhistory[0+player*5]==tmp[player])||((6!=tmp[player])&&(7!=tmp[player])&&(12!=tmp[player])&&(13!=tmp[player])&&(18!=tmp[player])&&(26!=tmp[player])));
			item_coming[player] = tmp[player];
			itemhistory[0+player*5]=tmp[player];
		}else if(use_item[player] == item_num+3){//TGM 1-5 16-25 28-31
			do{
				tmp2[player] = rand(item_pronum,player);
				tmp[player] = 1;
				do{
					tmp2[player] = tmp2[player]-item_pro[tmp[player]-1 ];
					if(tmp2[player] < 0){
						break;
					}
						tmp[player]++;
					if(tmp[player]>item_num+1)break;
				}while(1);
			//履歴の中にあったら引き直し
			}while((itemhistory[0+player*5]==tmp[player])||(itemhistory[1+player*5]==tmp[player])||(itemhistory[2+player*5]==tmp[player])||
					(itemhistory[3+player*5]==tmp[player])||(itemhistory[4+player*5]==tmp[player])||
					( ((tmp[player] > 5)&&(tmp[player] < 16)) || ((tmp[player] > 25)&&(tmp[player] < 28)) || (tmp[player] > 31) ));
			item_coming[player] = tmp[player];
			itemhistory[0+player*5]=itemhistory[1+player*5];//履歴をずらす
			itemhistory[1+player*5]=itemhistory[2+player*5];
			itemhistory[2+player*5]=itemhistory[3+player*5];
			itemhistory[3+player*5]=itemhistory[4+player*5];
			itemhistory[4+player*5]=tmp[player];
		}
		item_name[player] = item_coming[player];
		item_g[player] = 0;
	//novice100
	} else if((gameMode[player] == 0)&&(novice_mode[player])&&(tc[player] >= 100)&&(novice_flag[player]==0)){
		PlaySE(9);
		item_coming[player] = 28;
		item_name[player] = item_coming[player];//
		novice_flag[player]=1;
	//novice200
	} else if((gameMode[player] == 0)&&(novice_mode[player])&&(tc[player] >= 200)&&(novice_flag[player] ==1)){
		PlaySE(9);
		item_coming[player] = 19;
		item_name[player] = item_coming[player];
		novice_flag[player]=2;
	} else {
		item[player] = 0;
	}

	// 対戦モードでのブロック数制のアイテム効果終了
	if((gameMode[player] == 4) || (item_mode[player])) {
		if( (isfmirror[player]) && (item_t[player] > 3 - (repversw >=47)) ) {
			// MIRROR
			isfmirror[player] = 0;
			stopmirror_flag[player] = 0;
			item_t[player] = 0;
		} else if( (isrollroll[player]) && (item_t[player] > 3) ) {
			// ROLL ROLL
			isrollroll[player] = 0;
			item_rollroll[player] = 0;
			item_t[player] = 0;
		} else if( (IsBig[player]) && (!IsBigStart[player]) && (item_t[player] > 1) ) {
			// DEATH BLOCK
			IsBig[player] = 0;
			item_t[player] = 0;
		} else if( (isxray[player]) && (item_t[player] > 4) ) {
			// X-RAY
			isxray[player] = 0;
			xray_flag[player] = 0;
			item_t[player] = 0;
		} else if( (iscolor[player]) && (item_t[player] > 3) ) {
			// COLOR
			iscolor[player] = 0;
			color_flag[player] = 0;
			item_t[player] = 0;
		} else if( (isholdlock[player]) && (item_t[player] > 6) ) {
			// HOLD LOCK
			isholdlock[player] = 0;
			item_t[player] = 0;
		} else if( (ismagnetblock[player]) && (item_t[player] > 2) ) {
			// MAGNET BLOCK
			ismagnetblock[player] = 0;
			item_t[player] = 0;
		} else if( (isremote[player]) && (item_t[player] > 2) ) {
			// REMOTE CONTROL
			isremote[player] = 0;
			item_t[player] = 0;
		} else if( isUPdelfield[player] ) {
			// ↑DEL FIELD
			isUPdelfield[player] = 0;
		} else if( isDWdelfield[player] ) {
			// ↓DEL FIELD
			isDWdelfield[player] = 0;
		} else if( isdeleven[player] ) {
			// DEL EVEN
			isdeleven[player] = 0;
		} else if( (istrance[player]) && (item_t[player] > 3) ) {
			// TRANCE FORM
			istrance[player] = 0;
			item_t[player] = 0;
		} else if( isLmovfield[player] ) {
			// ←MOV FIELD
			isLmovfield[player] = 0;
		} else if( isRmovfield[player] ) {
			// →MOV FIELD
			isRmovfield[player] = 0;
		} else if( isallclear[player] ) {
			// ALLCLAER
			isallclear[player] = 0;
		}  else if( (ismiss[player]) && (misstimer[player] < 0) ) {
			// MISS
			ismiss[player] = 0;
			misstimer[player] = 0;
		} else if( (item_monochrome[player]) && (item_t[player] > 20) ) {
			// []
			item_monochrome[player] = 0;
			item_t[player] = 0;
		}
	}
	item_waiting[player] = 0;

		c_cblk[player] = c_nblk[0 + player * 6];

	c_cblk_r[player] = c_nblk[0 + player * 6];
	if(isfever[player]){	//フィーバー中は色も棒に変換
		if(isWRule(player)){
			c_cblk[player] = wcol[0];
			c_cblk_r[player] = wcol[0];
		} else if((rots[player] >= 4) && (rots[player] != 8)){
			c_cblk[player] = acol[0];
			c_cblk_r[player] = acol[0];
		} else{
			c_cblk[player] = ccol[0];
			c_cblk_r[player] = ccol[0];
		}
	}


	// NEXT領域内のブロック色をずらす
	setNextBlockColors(player, 0);

	if(isfever[player]){
		if(isWRule(player)){
			c_nblk[0 + player * 6] = wcol[0];
		} else if((rots[player] >= 4) && (rots[player] != 8)){
			c_nblk[0 + player * 6] = acol[0];
		} else{
			c_nblk[0 + player * 6] = ccol[0];
		}
	}
	if((ishidenext[player] == 0) && (!isfakenext[player])){//HIDENEXTの時は音も隠す
		PlaySE(21 + next[player]);
	}
	if(isfakenext[player]==1){//FAKENEXTの時は適当な音
		PlaySE(21 + rand(6,player));
	}
	if((!isrotatelock[player]) && (!ace_irs))
		rt[player] = 0;

	setBlockSpawnPosition(player);	//LITE版より移植　C7U0
	bs[player] = 0;
	bk[player] = 0;
	bk_D[player] = 0;	//D.R.S用

	ofs_x[player] = 0;
	ofs_x2[player] = 0;

	//NEXT遅延　1G以上でも出現した時点で接地していれば遅延させる
	if( (spawn_y_type) && (heboGB[player]==0) && (blk[player] != 0) ){
	 	if((rots[player] != 6) && (sp[player] < 60) ||
	 		(judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0))
	 		ndelay[player] = 0;
	}
	stat[player] = 5;

	// コンピュータが操作している場合は最善手を探す
	if(cpu_flag[player]) {
		cp_rot_c[player] = 0;
		cpuCheckBestSpot(player);
		if((!istrance[player]) && ((rots[player] != 6) && (rots[player] != 8)) && ((wait2[player] != 0) || (wait1[player] != 0))) cpuMove(player);
	}
	if((navigation) && (gameMode[player] == 0) && (tc[player] < navigation_limitLv)){
		cpuCheckBestSpot(player);
		navigation_time[player] = 0;
	}


	// I.H.S.	#1.60c
	doHold(player, 1);	// 別関数に独立 #1.60c7j3

	// (I.R.S.) Initial Rotation System  (c)1998 AR●KA(^^;
	if(!ace_irs)
		doIRS(player);		// 別関数に独立 #1.60c7j3

	// ブロックがめり込んでいたらゲームオーバー
	// ゲームオーバーなっていない＆接地している場合は音を鳴らす
	if( (!checkGameOver(player)) && (judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0) ) {
		if( (!isWRule(player)) || (world_drop_snd >= 1) ) PlaySE(1);
	}
	if(repversw >= 59){				// このバージョンから同一フレーム内に移動処理開始
		statc[player * 10 + 6] = 1;	// 同一フレームでは回転＆HOLDさせないYO
		if(stat[player] != 5) return;
		else statMove(player);		// 移動処理
	}
}


/* ゲームオーバーになるかチェック ゲームオーバーになった場合は1を返す */
// statBlockから独立 #1.60c7l2
int checkGameOver(int player) {
	// ブロックがめり込んでいたら
	if(judgeBlock(player, bx[player], by[player], blk[player], rt[player]) != 0) {
		setBlock(player, bx[player], by[player], blk[player], rt[player]);
		if((spawn_y_type) && (heboGB[player] == 0) || (heboGB[player] == 1)) ndelay[player] = 0;
		// ゲームオーバー処理
		setGameOver(player);

		return 1;
	}
	return 0;
}

/* ゲームオーバー処理 */
void setGameOver(int player) {
	int i;

	disableShadowTimer(player);		// シャドウタイマー解除 #1.60c7k7

	statusClear(player);
	ismiss[player] = 0;
	isreflect[player] = 0;
	isblind[player] = 0;
	blind_flag[player] = 0;

	hanabi_waiting[player] = 0;//hanabiもストップ
	hanabi_combo[player] = 0;//hanabiもストップ
	if(!(fastroll[player]) || (gameMode[player] == 5))
		staffInitPl(player);			// スタッフロール消去 #1.60c7n5

	if( (gameMode[player] <= 3) || (gameMode[player] == 5) ) {
		checkSecretGrade(player);	// 裏段位認定 #1.60c7o1
		if( (sgrade[player] >= min_sgrade) && (gameMode[player] != 5) ) PlaySE(17);
	}
	if(!((fastroll[player]) && (ending[player] == 2)) &&
		!((gameMode[player] == 9) && (relaymode[player]) && (!ending[player])) || (gameMode[player] == 5)){
		timeOn[player] = 0;				// タイマー停止
		if(gameMode[player] != 4) onRecord[player] = 0;			// リプレイ記録終了
	}

	if((gameMode[player] <= 1)&&(gameMode[player] >= 2)&&(gmflag_r[0]==1)&&(enable_grade[player] == 2)){//消えロール中に死んだら段位が1上がる
		grade[player]++;
		gflash[player]=120;
	}

	if((gameMode[player] <= 1)&&(gameMode[player] >= 2)&&(gmflag_r[0]==1)&&(enable_grade[player] == 3)){//消えロール中に死んだらM
		grade[player]=27;
		gflash[player]=120;
	}
	if((tc[player] - ((tc[player]/100)*100) > 50)&&(enable_grade[player] == 4)){//各セクションの後半(X50～X99)で死んだら
		grade[player] = grade[player] + gup3rank[player];

        // TODO: What should this be set to??
		//if(grade[player]>32)grade[player];
	}

	stat[player] = 7;				// ゲームオーバー画面へ
	if(!((fastroll[player]) && (ending[player] == 2)) &&
		!((gameMode[player] == 9) && (relaymode[player]) && (!ending[player])) || (gameMode[player] == 5)){
		if( (stat[1 - player] == 0) || (stat[1 - player] == 10) ) {
			if(wavebgm) {
				StopAllBGM();
			} else {
				if(IsPlayMIDI()) StopMIDI();
			}
		}
	}
	// ステータスカウンタ初期化1
	for(i = 0; i < 10; i++) statc[player * 10 + i] = 0;
	statc[player * 10] = -120 * (hidden[player] == 8);
}

/* 裏段位認定 #1.60c7o1 */
void checkSecretGrade(int player) {
	int i, j;
	int uhole, hmode;

	uhole = 0;		// 穴の位置 最初は一番左
	hmode = 0;		// 折り返しフラグ

	sgrade[player] = -1;	// 裏段位

	for(i = fldsizeh[player]; i > 3; i--) {	// 最上段とフィールド外はチェックしない
		for(j = 0; j < fldsizew[player]; j++) {
			// 穴の部分以外が埋まっていない場合は終了
			if( (j != uhole) && (fld[j + fldsizew[player] * i + player * 220] == 0) ) {
				return;
			}
		}

		// 穴の位置を変える
		if(!hmode) {
			uhole++;

			// 折り返し判定
			if(uhole >= fldsizew[player]) {
				uhole = fldsizew[player] - 2;
				hmode = 1;
			}
		} else {
			uhole--;
			if(uhole < 0) uhole = 0;
		}

		sgrade[player]++;	// 裏段位上昇
	}

	/* GM判定 */
	// 左上(X:0 Y:1)が埋まっていたらGM
	if(fld[1 + fldsizew[player] * 2 + player * 220] != 0) {
		sgrade[player]++;
	}
}

// (I.R.S.) Initial Rotation System  (c)1998 AR●KA(^^;
// #1.60c7j3
void doIRS(int player) {
	int bak;
	if((disable_irs) || (isrotatelock[player]) || (heboGB[player]>0)) return;

	if(repversw >= 43){	//新Ver
	if( getPressState(player, 4) || getPressState(player, 5) || getPressState(player, 6) ) {
		PlaySE(0);

		// 回転する方向を決める
		if( !getPressState(player, 5) ) {
			// AまたはC
			if(isWRule(player) && w_reverse)
				bak = 1;	// WORLD 右回転
			else
				bak = 3;	// CLASSIC 左回転
		} else {
			// B
			if(isWRule(player) && w_reverse)
				bak = 3;	// WORLD 左回転
			else
				bak = 1;	// CLASSIC 右回転
		}

		// 180度回転
		if( ((rots[player] == 7) || (rots[player] == 8)) && getPressState(player, 6) ) {
			bak = 2;
		}

		// 回転できるかチェック
		if( judgeBlock(player, bx[player], by[player], blk[player], bak) == 0 ) {
			// 回転可能
			if(rotate_snd) PlaySE(4);
			rt[player] = bak;
		} else if(!blk[player] && !r_irs) {
			// 赤色ブロックで回転しなかったときは1マス上にずらせるか判定
			if( judgeBlock(player, bx[player], by[player] - 1, blk[player], bak) == 0 ) {
				if(rotate_snd) PlaySE(4);
				rt[player] = bak;
				by[player]--;
			}
		}
	}
	}else{	//旧Ver
	// 回転方向逆転設定対応 激しく汚いorz #1.60c7f8
	if(getPressState(player, 4) || getPressState(player, 6)) {
		PlaySE(0);
		if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7

		// 回転逆転 #1.60c7f8
		if( (isWRule(player)) && (w_reverse) )
			rt[player] = (judgeBlock(player, bx[player], by[player], blk[player], 1 +
			(1*((rots[player] == 7) && getPressState(player, 6)))) == 0) +(1*((rots[player] == 7) && getPressState(player, 6)));
		else
			rt[player] = (judgeBlock(player, bx[player], by[player], blk[player], 3 -
			(1*((rots[player] == 8) && getPressState(player, 6)))) == 0) * 3 - (1*((rots[player] == 8) && getPressState(player, 6)));

		// 赤色ブロックで回転しなかったとき
		if(!rt[player] && !blk[player] && !r_irs) {
			// 回転逆転 #1.60c7f8
			if( (isWRule(player)) && (w_reverse) )
				rt[player] = (judgeBlock(player, bx[player], by[player] - 1, blk[player], 1 +(1
				*((rots[player] == 7) && getPressState(player, 6)))) == 0) +(1*((rots[player] == 7) && getPressState(player, 6)));
			else
				rt[player] = (judgeBlock(player, bx[player], by[player] - 1, blk[player], 3) == 0) * 3;

			if(rt[player])
				by[player]--;
		}
	}
	if(getPressState(player, 5)) {
		PlaySE(0);
		if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7

		// 回転逆転 #1.60c7f8
		if( (isWRule(player)) && (w_reverse) )
			rt[player] = (judgeBlock(player, bx[player], by[player], blk[player], 3) == 0) * 3;
		else
			rt[player] = (judgeBlock(player, bx[player], by[player], blk[player], 1) == 0);

		// 赤色ブロックで回転しなかったとき
		if(!rt[player] && !blk[player] && !r_irs) {
			// 回転逆転 #1.60c7f8
			if( (isWRule(player)) && (w_reverse) )
				rt[player] = (judgeBlock(player, bx[player], by[player] - 1, blk[player], 3) == 0) * 3;
			else
				rt[player] = (judgeBlock(player, bx[player], by[player] - 1, blk[player], 1) == 0);

			if(rt[player])
				by[player]--;
		}
	}
	}
}
// ACE式IRS
void doIRS2(int player) {
	if((disable_irs) || (isrotatelock[player]) || (heboGB[player]>0)) return;

	// 回転方向逆転設定対応 激しく汚いorz #1.60c7f8
	if(getPushState(player, 4) || getPushState(player, 6)) {
		PlaySE(0);
		if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7

		// 回転逆転 #1.60c7f8
		if( (isWRule(player)) && (w_reverse) ){
			rt_nblk[0 + 6 * player]++;
			if((rots[player] == 7) && (getPushState(player , 6) != 0))
				rt_nblk[0 + 6 * player]++;
		}else{
			rt_nblk[0 + 6 * player]--;
			if(((rots[player] == 8) || (rots[player] == 7)) && (getPushState(player , 6) != 0))
				rt_nblk[0 + 6 * player]--;
		}
		if(rt_nblk[0 + 6 * player] > 3) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] - 4;
		if(rt_nblk[0 + 6 * player] < 0) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] + 4;
	}
	if(getPushState(player, 5)) {
		PlaySE(0);
		if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7

		// 回転逆転 #1.60c7f8
		if( (isWRule(player)) && (w_reverse) )
			rt_nblk[0 + 6 * player]--;
		else
			rt_nblk[0 + 6 * player]++;

		if(rt_nblk[0 + 6 * player] > 3) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] - 4;
		if(rt_nblk[0 + 6 * player] < 0) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] + 4;
	}
}
//ACEカスタムIRS ブロック固定の瞬間のみ
void doIRS2plus(int player) {
	if((disable_irs) || (isrotatelock[player]) || (heboGB[player]>0)) return;

	// 回転方向逆転設定対応 激しく汚いorz #1.60c7f8
	if(getPressState(player, 4) || getPressState(player, 6)) {
		PlaySE(0);
		if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7

		// 回転逆転 #1.60c7f8
		if( (isWRule(player)) && (w_reverse) ){
			rt_nblk[0 + 6 * player]++;
			if((rots[player] == 7) && (getPressState(player , 6) != 0))
				rt_nblk[0 + 6 * player]++;
		}else{
			rt_nblk[0 + 6 * player]--;
			if((rots[player] == 8) && (getPressState(player , 6) != 0))
				rt_nblk[0 + 6 * player]--;
		}
		if(rt_nblk[0 + 6 * player] > 3) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] - 4;
		if(rt_nblk[0 + 6 * player] < 0) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] + 4;
	}
	if(getPressState(player, 5)) {
		PlaySE(0);
		if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7

		// 回転逆転 #1.60c7f8
		if( (isWRule(player)) && (w_reverse) )
			rt_nblk[0 + 6 * player]--;
		else
			rt_nblk[0 + 6 * player]++;

		if(rt_nblk[0 + 6 * player] > 3) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] - 4;
		if(rt_nblk[0 + 6 * player] < 0) rt_nblk[0 + 6 * player] = rt_nblk[0 + 6 * player] + 4;
	}
}
// ブロック出現位置を設定　From LITE Ver
void setBlockSpawnPosition(int player) {
	if((spawn_y_type) && (heboGB[player]==0)) {
		// 枠外
		if(IsBig[player]) {
			// BIG
			bx[player] = 2;

			if(isWRule(player)) {
				by[player] = (blk[player] == 0) * -1;
			} else {
				by[player] = -1 - (blk[player] != 0) - (2 * ((blk[player] == 0) && (rots[player] == 8)));
			}
		} else {
			// 通常
			bx[player] = 3;

			if(isWRule(player)) {
				by[player] = 0;
			} else {
				by[player] = ((blk[player] != 0)  || (rots[player] == 8)) * -1;
			}
		}
	} else {
		// 枠内
		if(IsBig[player]) {
			// BIG
			bx[player] = 2;
			by[player] = (( isWRule(player) && (blk[player] != 0) ) * 2)  - 2 * ((blk[player] == 0 ) && (rots[player] == 8));
		} else {
			// 通常
			bx[player] = 3;
			by[player] = 1 + ( (blk[player] != 0) && isWRule(player) ) - 1 * ((blk[player] == 0 ) && (rots[player] == 8));
		}
	}
	// D.R.Sの先行移動
	if(rots[player] == 8){
		if(getPressState(player, 2) && (judgeBlock(player, bx[player] - 1 - (1 * (IsBig[player] && BigMove[player])), by[player], blk[player], rt[player]) == 0)){
			bx[player] = bx[player] - 1 - (1 * (IsBig[player] && BigMove[player]));
			if(movesound) PlaySE(5);
		}
		if((blk[player] != 0) || (!IsBig[player]))
		if(getPressState(player, 3) && (judgeBlock(player, bx[player] + 1 + (1 * (IsBig[player] && BigMove[player])), by[player], blk[player], rt[player]) == 0)){
			bx[player] = bx[player] + 1 + (1 * (IsBig[player] && BigMove[player]));
			if(movesound) PlaySE(5);
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.05 - ブロック移動中
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statMove(int player) {
	int tmp;
	// 注意：実際の処理はこのファイルにはありません。
	// ワールドルールの場合はworld.c、クラシックルールの場合はclassic.cに分岐します。

	// CPUが操作している場合は入力内容を決める #1.60c7o2
	if((cpu_flag[player]) && (statc[player * 10 + 6] != 1)) {
		cpuMove(player);
	}
	hanabi_flag=0;

	// NEXTブロック表示の遅延
	if(!IsBig[player]) {
		if(isWRule(player)) {
			tmp = (blk[player] != 0);
		} else if(rots[player] == 8) {
			tmp = (blk[player] == 0) * -1;
		} else {
			tmp = 0;
		}
	} else {
		if(isWRule(player)) {
			if(blk[player] == 0) tmp = -1;
			else tmp = 1;
		} else if(rots[player] == 8) {
			tmp = -1 + (blk[player] == 0) * -2;
		} else {
			tmp = -1;
		}
	}

	if((by[player] >= tmp) || (!spawn_y_type) || (rots[player] == 6))
		ndelay[player] = 1;

	if(rots[player] == 2) {
		statWMove( player,  10 -2 + (repversw >= 16) , 8);	// world.c
	} else if(rots[player] == 3) {
		statWMove( player, 128 -2 + (repversw >= 16) - (108*(gameMode[player] == 4)),128 - (108*(gameMode[player] == 4)));	// WORLD2 #1.60c7o
	} else if(rots[player] == 4) {
		statAMove( player, 128 -2 + (repversw >= 16) - (108*(gameMode[player] == 4)),128 - (108*(gameMode[player] == 4)));	// ARS #1.60c7q2ex
	} else if(rots[player] == 5) {
		statAMove( player, 128 -2 + (repversw >= 16) - (113*(gameMode[player] == 4)),128 - (108*(gameMode[player] == 4)));	// ARS2 #1.60c7q2ex
	} else if(rots[player] == 6) {
		statWMove( player, -1 +(14*(gameMode[player] == 4)),-1+(14*(gameMode[player] == 4)));						// WORLD3 #1.60c7r2
	} else if(rots[player] == 7) {
		if(repversw < 39)
			statWMove( player, 128 -2 + (repversw >= 16),128);	// SRS-X 38まで
		else
			statWMove( player, 18 + (6 * (repversw >= 40)) -1 - (8*(gameMode[player] == 4)),12);	// SRS-X
	} else if(rots[player] == 8) {
		statDMove(player);	// D.R.S
	} else {
		statCMove(player);								// classic.c
	}
	if(spawn_y_type)// フィールド枠
		viewFldFrame(1,player);

	if((stat[player] != 5) && (repversw >= 57)) return;

	if(timeOn[player]) {
		timeN[player]++;	//gradeup3で使う
	}
	if((navigation) && (gameMode[player] == 0) && (tc[player] < navigation_limitLv))
		navigation_time[player]++;

}

// HOLDする処理 #1.60c7j3
void doHold(int player, int ihs) {
	int tmp,first;
	first = 0;
	if((disable_hold) || (isholdlock[player] == 1) || (death_plus[player])||(hebo_plus[player])||(heboGB[player])) return;

	if(getPressState(player, 7) && !dhold[player]) {
		if( (hold_snd != 0) && (((hold_snd == 1) && (ihs)) || (hold_snd == 2)) )
		PlaySE(6);	// hold.wav #1.60c7f7

		if(hold[player] == -1) {//空っぽ
			first = 1;
			hold[player] = blk[player];
			blk[player] = next[player];
			nextc[player] = (nextc[player] + 1) % 1400;
			next[player] = nextb[nextc[player] + player * 1400];
			hold_used[player] = 1; // 回数 #1.60c7i1

			// ブロックの色設定 #1.60c7m9
			c_hblk[player]   = c_cblk_r[player];

			c_cblk[player] = c_nblk[0 + player * 6];

			c_cblk_r[player] = c_nblk[0 + player * 6];

			item_coming[player] = 0;
			if(ace_irs == 2) tmp = rt[player];
			// NEXT領域内のブロック色をずらす
			setNextBlockColors(player, 0);
			if(ace_irs == 2) rt[player] = tmp;
//			scanItem(player);
		} else {
			dhold[player] = blk[player];
			dhold2[player] = blk[player];
			blk[player] = hold[player];
			hold[player] = dhold[player];

			hold_used[player] = 2; // 回数 #1.60c7i1

			// ブロックの色設定 #1.60c7m9
			tmp = c_cblk_r[player];

			//c_cblk[player]   = (c_hblk[player]) * (!isrollroll[player]);
			if(isrollroll[player]) {
				// []対応 #1.60c7p9ex改造
				c_cblk[player] = (c_hblk[player] == 8) * 9;
			} else {
				c_cblk[player] = c_hblk[player];
			}

			c_cblk_r[player] = c_hblk[player];
			c_hblk[player]   = tmp;
		}
		dhold[player] = 1;
		dhold2[player] = 1;
		kickc[player] = 0;
		kickc2[player] = 0;
		kickc3[player] = 0;

		if(cpu_flag[player]) {
			cp_rot_c[player] = 0;
			cpuCheckBestSpot(player);
		}
		if((navigation) && (gameMode[player] == 0) && (tc[player] < navigation_limitLv)){
			cpuCheckBestSpot(player);
			navigation_time[player] = 0;
		}

		setBlockSpawnPosition(player);	//LITE版より移植　C7U0

		//NEXT遅延　1G以上でも出現した時点で接地していれば遅延させる
		if( (spawn_y_type) && (heboGB[player]==0) && (blk[player] != 0) ){
		 	if((rots[player] != 6) && (sp[player] < 60) ||
		 		(judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0))
		 		ndelay[player] = 0;
		}

		if((!first) && !((ihs) && (ace_irs == 2)) || ((!ace_irs) && (repversw >=47)))
			rt[player] = 0;
		if(repversw >= 21){
		bs[player] = 0;			// ブロック落下をリセット C7T7.101
		bk[player] = 0;			// ブロック落下をリセット C7T7.101
		bk_D[player] = 0;
		}

		if((item[player] != 0) && (gameMode[player] == 4) && (item[player] != fldihardno) && (item[player] != 36) && ((!first) || (repversw < 48))) {
			item[player] = 0;	// アイテム消去 #1.60c7n1
			scanItem(player);
		}
		// #1.60c7j3からIHSでないタイミングでもIRSを受け付ける
		if((!ihs) && (repversw >= 7)) {
			if(ace_irs != 1){
				doIRS(player);
			}

			// もう一度blockseを鳴らす #1.60c7o1
			if((ishidenext[player] == 0) && ((repversw < 64) || (isfakenext[player] == 0))){
				PlaySE(21 + next[player]);
			}
			if(isfakenext[player] == 1){//FAKENEXTの時は適当な音
				PlaySE(21 + rand(6,player));
			}
			// 横方向の先行移動を無効にする #1.60c7o9
			if(repversw >= 11) {
				statc[player * 10 + 4] = (fastlrmove == 0);
				if(rots[player] == 8) statc[player * 10 + 4] = 0;
			}

			// 固定までの時間をリセット #1.60c7p5
			if(repversw >= 13){
				bk[player] = 0;
				bk_D[player] = 0;
			}
		}

		// ブロックがめり込んでいたらゲームオーバー #1.60c7l2
		// ゲームオーバーなっていない＆接地している場合は音を鳴らす
		if( (!checkGameOver(player)) && (judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0) ) {
			if( (!isWRule(player)) || (world_drop_snd >= 1) ) PlaySE(1);
		}
	}
}

// TLSが有効か判定
// クラシックとワールドで分かれていたものを一本化 #1.60c7j6
int isTLS(int player) {
	// noredrawが1だったらTLSなし
	if(noredraw) {
		return 0;
	}

	//オールドスタイルは常に無効
	if(heboGB[player]){
		return 0;
	}

	// tls_viewが1だったら常に有効
	if(tls_view == 1) {
		return 1;
	}

	// tls_viewが2だったら常に無効
	if(tls_view == 2) {
		return 0;
	}

	//フィーバー中は常に有効
	if(isfever[player]) {
		return 1;
	}

	// BEGINNERは常に有効
	if(gameMode[player] == 0) {
		return 1;
	}

	// MASTER
	if(gameMode[player] == 1) {
		// TGM式レベルアップではLV100まで
		if(lv[player] < 100) {
			return 1;
		}

	}

	// VERSUS
	if(gameMode[player] == 4) {
		// とりあえずLV100まで
		if(lv[player] < 100) {
			return 1;
		}
	}

	// PRACTICE
	if(gameMode[player] == 5) {
		// 設定が「LV100」
		if((tlsrange == 1) && (lv[player] < 100)) {
			return 1;
		}
		// 設定が「ALWAYS」
		if(tlsrange == 2) {
			return 1;
		}
	}

	// tomoyo #1.60c7l5
	if(gameMode[player] == 6) {
		// とりあえずLV100まで
		if(lv[player] < 100) {
			return 1;
		}
	}

	// ACE
	if( (gameMode[player] == 7) || (gameMode[player] == 8) ) {
		if(heboGB[player] >0){
			return 0;
		}else{
			return 1;
		}
	}
		// ORI
	if(gameMode[player] == 10) {
		// BEGINNERは常に有効
		if(ori_opt[player] == 0) {
			return 1;
		}else{
			if(lv[player] < 100) {
				return 1;
			}
		}
	}

	// 上のどれにも当てはまらないならTLSはなし
	return 0;
}

// T-SPIN判定 #1.60c7s6
// BIG対応
int isTSpin(int player) {
	int i, count, tx[4], ty[4];
	int tmp_x, tmp_y;

	// 判定用相対座標を設定
	if(IsBig[player]){
		tx[0] = 1;
		ty[0] = 1;
		tx[1] = 4;
		ty[1] = 1;
		tx[2] = 1;
		ty[2] = 4;
		tx[3] = 4;
		ty[3] = 4;
	} else {
		tx[0] = 0;
		ty[0] = 0;
		tx[1] = 2;
		ty[1] = 0;
		tx[2] = 0;
		ty[2] = 2;
		tx[3] = 2;
		ty[3] = 2;
	}

	// 判定処理
	count = 0;

	for(i = 0; i < 4; i++) {
		tmp_x = bx[player] + tx[i];
		tmp_y = by[player] + ty[i];

		//ARSでTが上向きの場合
		//◇　◇
		//　■
		//◆■◆
		//こんな判定だったのを修正
		if((isWRule(player) == 0) && (rt[player] == 2))
			tmp_y = tmp_y + 1 + (IsBig[player]);

		if(tmp_y >= 0) {
			if( (tmp_x < 0) || (tmp_x >= 10) || (tmp_y >= 22) || (fld[tmp_x + tmp_y * 10 + player * 220]) ) {
				count++;
			}
		}
	}

	return count;
}

// ブロック消去判定 fldsize対応 #1.60c7
// 消去される場合、1を返す #1.60c7k8
int blockEraseJudge(int player) {
	int		i, j, k, l,sr,ret;
	int		by2;	// 調べるY座標

//	if(IsBig[player]) {
//		// BIGでは専用処理
//		return blockBigEraseJudge(player);
//	}

	// ラインが埋まってるか判定
	ret = 0;
	for(i = 0; i <= fldsizeh[player]; i++) {
		sr = 1;
		for(j = 0; j < fldsizew[player]; j++)
			if(fld[j + i * fldsizew[player] + player * 220] == 0) {//揃ってないので消えません
				sr = 0;
				break;
			}
		erase[i + player * 22] = sr;
		if (sr) ret++;	//消去あり
	}

	// 対戦　DEFENCE
	// 底から「消えるライン数-1ライン」が消去される
	sr = 0;
	if((ret >= 2) && (gameMode[player] == 4) && (vs_style[player] == 2)){
		for(i = fldsizeh[player]; i > 0; i--){
			if(erase[i + player * 22] == 0){
				erase[i + player * 22] = 2;
				sr++;
			}
			if(sr >= ret - 1) break;
		}
	}

	// 対戦　MOVE ERASEDせり上がり
	// 「相手が」MOVE ERASED式の場合に処理を行う
	// ここではせり上がりフィールドには入れず、バッファへ確保
	if((ret != 0) && ((gameMode[player] == 4) && (upLineT[1 - player] == 3)) && (!disrise)){
		removeBlock(player, bx[player], by[player], blk[player], rt[player]);
		// 対戦　ATTACK
		if(vs_style[player] == 1){
			if(ret >= 4) sr = 2;
			else sr = 1;
			k = 0;
			for(i = 0; i <= fldsizeh[player]; i++) {
				if(erase[i + player * 22] == 1){
					for(j = 0; j < fldsizew[player]; j++) {
						fldubuf[j + upLines_waiting[1 - player] * fldsizew[player] + (1 - player) * 220] = fld[j + i * fldsizew[player] + player * 220];
					}
					k++;
					upLines_waiting[1 - player]++;
				}
				if(upLines_waiting[1 - player] + 1 >= 21) break;
				if(k >= sr) break;
			}
		}
		for(i = 0; i <= fldsizeh[player]; i++) {
			if(erase[(fldsizeh[player] - i) + player * 22] == 1) {
				for(j = 0; j < fldsizew[player]; j++) {
					fldubuf[j + upLines_waiting[1 - player] * fldsizew[player] + (1 - player) * 220] = fld[j + (fldsizeh[player] - i) * fldsizew[player] + player * 220];
				}
				upLines_waiting[1 - player]++;
				if(upLines_waiting[1 - player] + 1 >= 21) break;
			}
		}
		setBlock(player, bx[player], by[player], blk[player], rt[player]);
	}
	return ret;
}

/* BIG用消去判定 */
int blockBigEraseJudge(int player) {
	int		i, j, sr,ret;

	ret = 0;
	for(i = 0; i <= fldsizeh[player]; i++) {
		sr = 1;
		for(j = 0; j < fldsizew[player]; j++)
			if(fld[j + i * fldsizew[player] + player * 220] == 0) {
				sr = 0;
				break;
			}
		erase[i + player * 22] = sr;
		if (sr) ret = 1;	//消去あり
	}

	if((ret) && ((gameMode[player] == 4) && (upLineT[1 - player] == 3)) && (!disrise)){
		removeBlock(player, bx[player], by[player], blk[player], rt[player]);
		for(i = 0; i <= fldsizeh[player]; i++) {
			if(erase[(fldsizeh[player] - i) + player * 22]) {
				for(j = 0; j < fldsizew[player]; j++) {
					fldubuf[j + upLines_waiting[1 - player] * fldsizew[player] + (1 - player) * 220] = fld[j + (fldsizeh[player] - i) * fldsizew[player] + player * 220];
				}
				upLines_waiting[1 - player]++;
				if(upLines_waiting[1 - player] + 1 >= 21) break;
			}
		}
		setBlock(player, bx[player], by[player], blk[player], rt[player]);
	}

	return ret;
}

/* FREE FALL用消去判定 */
int blockEraseJudgeFf(int player,int mode) {
	int		i, j, k, l, sr, ret, hole;
	if(mode == 0){
		ret = 0;
		for(i = 0; i <= fldsizeh[player]; i++) {
			sr = 1;
			for(j = 0; j < fldsizew[player]; j++)
				if(fld[j + i * fldsizew[player] + player * 220] == 0) {
					sr = 0;
					break;
				}
			erase[i + player * 22] = sr;
			if (sr) ret = 1;	//消去あり
		}
		return ret;
	}else{
		if((upLineT[1 - player] == 3) && (!disrise)){
			hole = rand(10,1 - player);
			//バッファからせり上がりフィールドへ送る
			for(i = 0; i < Ff_rerise[player]; i++) {
				for(j = 0; j < fldsizew[player]; j++) {
					if(j != hole)
						fldu[j + upLines[1 - player] * fldsizew[player] + (1 - player) * 220] = rand(7,1-player) + 2;
					else
						fldu[j + upLines[1 - player] * fldsizew[player] + (1 - player) * 220] = 0;
				}
				if(upLines[1 - player] < 21) upLines[1 - player]++;
			}
		}else if(!disrise){
			upLines[1 - player] = upLines[1 - player] + Ff_rerise[player];
		}

	}
	return 0;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.15 - ROTリレー用セレクト
//　どう見ても偽SHIRASEには使用されていなかったので再利用
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0] カウンタ
// statc[player * 10 + 1] カウンタ2
// statc[player * 10 + 2] 0なら演出中 1なら回転法則選択中
// statc[player * 10 + 3] 0ならクリア演出　1なら窒息演出
void statRelayselect(int player) {
	int block,i,j,c;

	timeOn[player] = 0;
	ndelay[player] = 1;

	pinch[player] = 0;

	padRepeat(player);
	padRepeat2(player);

	if(statc[player * 10 + 2] == 0){
		//警告音が鳴っていたら止める
		StopSE(40);
		StopSE(32);
		if(statc[player * 10 + 3] == 0){	//クリア演出
			if(statc[player * 10] == 0){
				PlaySE(18);
			}
			printFont(17 + 24 * player - 12 * maxPlay, 14, "GREAT!", (count % 4 / 2) * digitc[rots[player]]);
			// ブロックを消す
			// BIG対応 #1.60c7j5
			if(statc[player * 10] % 6 == 0) {
				j = (fldsizeh[player] + 1) - (statc[player * 10] / 6);
				if((j >= 0) && (j <= fldsizeh[player])) {
					for(i = 0; i < fldsizew[player]; i++) {
						// ライン消しエフェクトで消える #1.60c7n5
						if( fld[i+ j * fldsizew[player] + player * 220] != 0) {
							objectCreate(player, 1, (i + 15 + 24 * player - 12 * maxPlay) * 8,(j + 3) * 8, (i - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + 1 * 250, fld[i+ j * fldsizew[player] + player * 220], 100);
						}

						fld[i+ j * fldsizew[player] + player * 220] = 0;
						fldt[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7j5
						fldi[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7n1
						flds[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7n1
					}
				}
			}
			statc[player * 10]++;
			// アニメ終了
			if(statc[player * 10] == 132) {
				for(i = 0; i < 22; i++)
					erase[i + player * 22] = 0;
				statc[player * 10 + 2] = 1;
			}
		}else{	//窒息演出

			statc[player * 10 + 1]++;
			printFont(16 + 24 * player - 12 * maxPlay, 14, "TOP OUT!", (count % 4 / 2) * 2);
			if(std_opt[player] == 0){
				printFont(16 + 24 * player - 12 * maxPlay, 16, "+", (count % 4 / 2) * 2);
				getTime((40-li[player]) * 240);
				printFont(17 + 24 * player - 12 * maxPlay, 16, string[0], (count % 4 / 2) * 2);
			}else{
				sprintf(string[0],"-%3d LINES",li[player] / 2);
				printFont(15 + 24 * player - 12 * maxPlay, 16, string[0], (count % 4 / 2) * 2);
			}
			if( statc[player * 10] > 22 ) {
				if(statc[player * 10 + 1] == 60) {
					for(i = 0; i < 22; i++)
						erase[i + player * 22] = 0;
					statc[player * 10 + 2] = 1;
				}
			}
			else if(statc[player * 10 + 1] > 4) {
				block = statc[player * 10];
				if(block == 0) {
					PlaySE(45);
					if((deadtype) || (heboGB[player])) PlaySE(7);	// 下から消去は音はなし #1.60c7h3
				}

				statc[player * 10 + 1] = 0;
				for(j = 0; j < 10; j++) {
					// ブロックの色変更で、LV1000ブロックはほかのブロックとは別のものにする。 #1.60c7d
					// 死亡アニメを選べるようにした#1.60c7h3
					if((deadtype) || (heboGB[player])) {
						if(block <= fldsizeh[player]){
							c = fld[j + block * 10 + player * 220];
							if(c != 0) {
								if((c >= 9) || (c < 0))
									fld[j + block * 10 + player * 220] = 10;
								else
									fld[j + block * 10 + player * 220] = 1;
							}
						}
					} else {
						// スーッと消えていく演出 #1.60c7k6
						// +=のような代入演算子はつかえねぇ‥‥cond ? true : falseも駄目‥‥
						if(21 - block >= 0) {
							c = fldt[j + (210 - (block * 10)) + player * 220];
							if((c <= -1) || (c > 8)) c = 8;
							fldt[j + (210 - (block * 10)) + player * 220] = c - 4;
						}
						if(block > 0) {
							c = fldt[j + (210 - (block - 1) * 10) + player * 220];
							fldt[j + (210 - (block - 1) * 10) + player * 220] = c - 4;
						}
					}
				}
				statc[player * 10]++;
			}

		}
		if(statc[player * 10 +2] == 1){
			statc[player * 10] = 0;
			for(i = 0; i < 10; i++)
				for(j = 0; j < 22; j++) {
					fld[i + j * 10 + player * 220] = 0;
					fldt[i + j * 10 + player * 220] = 0;	// #1.60c7j5
					fldi[i + j * 10 + player * 220] = 0;	// #1.60c7n1
					flds[i + j * 10 + player * 220] = 0;
				}
			if(std_opt[player] == 0){
				if(statc[player * 10 + 3] == 0)
					relaydata[player * 9 + rots[player]] = time[player];
				else
					relaydata[player * 9 + rots[player]] = time[player] + (40-li[player]) * 240;
			}else{
				if(statc[player * 10 + 3] == 0)
					relaydata[player * 9 + rots[player]] = li[player];
				else
					relaydata[player * 9 + rots[player]] = li[player] - li[player] / 2;
				ltime[player] = 60 * 120;
			}
			time[player] = 0;
			li[player] = 0;
			relayround[player]++;
			bdowncnt[player] = 0;
			hold[player] = -1;
			hold_used[player] = 0;
			bgfadesw = 1;
			if(relayround[player] >= 9){	//エンディング
				PlaySE(28);
				c = 0;
				for(i = 0;i < 9;i++)
					c = c + relaydata[player * 9 + i];
				if(std_opt[player] == 0) time[player] = c;
				else li[player] = c;
				ending[player] = 1;
				stat[player] = 13;
				end_f[player] = 2;
				onRecord[player] = 0;
				return;
			}
		}
	}else{	// 次の回転法則を選ぶ
		if(statc[player * 10] == 0){
			do {
				rots[player]++;
				if(rots[player] > 8) rots[player] = 0;
			} while(relaydata[player * 9 + rots[player]] != -1);
			setNextBlockColors(player, 1);
		}

		printFont(15 + 24 * player - 12 * maxPlay, 5, "SELECT", 4);
		printFont(15 + 24 * player - 12 * maxPlay, 6, " NEXT ROT.", 4);

		printFont(15 + 24 * player - 12 * maxPlay, 7 + (2 * rots[player]), "b", (count % 2) * fontc[rots[player]]);

		printFont(16 + 24 * player - 12 * maxPlay, 7, "HEBORIS"  ,(relaydata[player * 9 + 0] == -1) * fontc[0]);
		printFont(16 + 24 * player - 12 * maxPlay, 9, "TI-ARS"   ,(relaydata[player * 9 + 1] == -1) * fontc[1]);
		printFont(16 + 24 * player - 12 * maxPlay, 11, "TI-WORLD",(relaydata[player * 9 + 2] == -1) * fontc[2]);
		printFont(16 + 24 * player - 12 * maxPlay, 13, "ACE-SRS" ,(relaydata[player * 9 + 3] == -1) * fontc[3]);
		printFont(16 + 24 * player - 12 * maxPlay, 15, "ACE-ARS" ,(relaydata[player * 9 + 4] == -1) * fontc[4]);
		printFont(16 + 24 * player - 12 * maxPlay, 17, "ACE-ARS2",(relaydata[player * 9 + 5] == -1) * fontc[5]);
		printFont(16 + 24 * player - 12 * maxPlay, 19, "DS-WORLD",(relaydata[player * 9 + 6] == -1) * fontc[6]);
		printFont(16 + 24 * player - 12 * maxPlay, 21, "SRS-X"   ,(relaydata[player * 9 + 7] == -1) * fontc[7]);
		printFont(16 + 24 * player - 12 * maxPlay, 23, "D.R.S"   ,(relaydata[player * 9 + 8] == -1) * fontc[8]);

		for(i = 0;i < 9;i++){
			if(relaydata[player * 9 + i] == -1){
				printFont(17 + 24 * player - 12 * maxPlay, 8 + (2 * i), "STAND BY", (count % 4 / 2) * digitc[i]);
			}else{
				if(std_opt[player] == 0){
					getTime(relaydata[player * 9 + i]);
					printFont(17 + 24 * player - 12 * maxPlay, 8 + (2 * i), string[0], 0);
				}else {
					sprintf(string[0],"%3dLINES",relaydata[player * 9 + i]);
					printFont(17 + 24 * player - 12 * maxPlay, 8 + (2 * i), string[0], 0);
				}
			}
		}

		sprintf(string[0], "%2d", 10 - (statc[player * 10] / 60));
		printFont(17 + 24 * player - 12 * maxPlay, 25, "TIME", 6);
		printFont(21 + 24 * player - 12 * maxPlay, 25, string[0], 0);

		// ↑
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) ){
		if(getPressState(player, 0)) {
			PlaySE(5);
			do {
				rots[player]--;
				if(rots[player] < 0) rots[player] = 8;
			} while(relaydata[player * 9 + rots[player]] != -1);
			setNextBlockColors(player, 1);
		}
		}

		// ↓
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) ){
		if(getPressState(player, 1)) {
			PlaySE(5);
			do {
				rots[player]++;
				if(rots[player] > 8) rots[player] = 0;
			} while(relaydata[player * 9 + rots[player]] != -1);
			setNextBlockColors(player, 1);
		}
		}


		statc[player * 10]++;

		if((getPushState(player, 4)) || (statc[player * 10] > 600)){
			PlaySE(10);
			setNextBlockColors(player, 1);
			stat[player] = 3;					// Ready
			statc[player * 10] = 0;				// ステータスカウンタを0に
			statc[player * 10 + 1] = 0;			//
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 3] = 0;
		}
	}

}

// ブロック上昇する?
void UpLineBlockJudge(int player) {
	int	j;
	// せり上がりラインが残っているときはせり上げる
	if(upLines[player] > 0) {
		// 対戦では本家っぽく着地直後のみせり上げてみる
		if((gameMode[player] == 4) && (stat[player] == 5)) return;

		UpLineBlock(player);

		by[player] = by[player] - (by[player] >= 0);

		// 枠内出現は22段目のブロックを消す C7U0
		if((!spawn_y_type) || (repversw <= 27)){
		for(j = 0; j < fldsizew[player]; j++) {
			fld[j + 0 * fldsizew[player] + player * 220] = 0;
			fldt[j + 0 * fldsizew[player] + player * 220] = 0;
			flds[j + 0 * fldsizew[player] + player * 220] = 0;
		}
		// ブロックが重なってしまったときは、ムリヤリ上げる
		if(stat[player] == 5)
			if(judgeBlock(player, bx[player], by[player], blk[player], rt[player])) by[player]--;
		}
	}
}

// ブロック上昇
void UpLineBlock(int player) {
	// SHIRASE以外のせり上がりタイプもBIGに対応 #1.60c7g3
	// #1.60c7j5
	// BIG専用処理を削除（BIGの仕様変更で不要に）#1.60c7o9
	int		i, j,newhole,h1,h2,riseT;

	riseT = upLineT[player];
	if((gameMode[player] == 8) && (mission_type[c_mission] == 21) && (stat[player] == 6))
		riseT = 2;

	if((gameMode[player] == 6) || ((gameMode[player] == 8) && (mission_type[c_mission] == 6))){	// TOMOYO
		PlaySE(20);
		//上にずらす
		for(i = 1; i <= fldsizeh[player]; i++) {
			for(j = 0; j < fldsizew[player]; j++) {
				fld[j + (i - 1) * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
				fldt[j + (i - 1) * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
				fldi[j + (i - 1) * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
				flds[j + (i - 1) * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
			}
			erase[(i - 1) + player * 22] = erase[i + player * 22];
		}

		// せり上がりフィールドの読み込み
		fld[0 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[0+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[1 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[1+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[2 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[2+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[3 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[3+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[4 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[4+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[5 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[5+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[6 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[6+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[7 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[7+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[8 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[8+(1*upLineNo[player] * fldsizew[player])+player*200];
		fld[9 + fldsizew[player] * fldsizeh[player] + player * 220] = tomoyo_rise_fld[9+(1*upLineNo[player] * fldsizew[player])+player*200];

		for(j = 0; j < fldsizew[player]; j++) {
			fldt[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j + fldsizew[player] * fldsizeh[player] + player * 220] != 0) * -1;
		}

		erase[fldsizeh[player] + player * 22] = 0;
		upLines[player]--;
		upLineNo[player]++;
		if(upLineNo[player] > 20)
			tomoyo_rise[player] = 0;
		else{
			j=0;
			for(i=0;i<fldsizew[player];i++){
				if(tomoyo_rise_fld[i+(1*upLineNo[player] * fldsizew[player])+player*200] != 0)
					break;
				j++;
			}
			if(j==fldsizew[player])
				tomoyo_rise[player] = 0;
		}

	} else if(riseT == 0) { // コピー7
		PlaySE(20);

		// フィールドを上にずらす
		for(i = 1; i <= fldsizeh[player]; i++) {
			for(j = 0; j < fldsizew[player]; j++) {
				fld[j + (i - 1) * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
				fldt[j + (i - 1) * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
				fldi[j + (i - 1) * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
				flds[j + (i - 1) * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
			}
			erase[(i - 1) + player * 22] = erase[i + player * 22];
		}

		// 下から2番目を一番下にコピーする
		for(j = 0; j < fldsizew[player]; j++) {
			fld[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j +fldsizew[player] * (fldsizeh[player] - 1) + player * 220] != 0);
			fldt[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j + fldsizew[player] * fldsizeh[player] + player * 220] != 0) * -1;
			if(hidden[player] == 8) fldt[j + fldsizeh[player] * fldsizew[player] + player * 220] = -60;
				// アイテムを増殖させない #1.60c7o6
				fldi[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
				flds[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
		}

		erase[fldsizeh[player] + player * 22] = 0;
		upLines[player]--;
	} else if(riseT == 1) {	// せり上がりタイプがTGM+のときせり上がりフィールドを読み込む#1.60c6.2g
		j = upLineNo[player];
		for(i = 0; i < upLineNoMax; i++) {
			j = i + upLineNo[player];
			if(j >= upLineNoMax) j = j - upLineNoMax;

			if(!((upFld[j* 2] == 00000) && (upFld[j* 2 + 1] == 00000)) &&	// せり上がりフィールドのチェック
				!((upFld[j* 2] == 11111) && (upFld[j* 2 + 1] == 11111))) {
				upLineNo[player] = j;
				break;
			}
		}
		if((i >= upLineNoMax) || (upLineNoMax == 0)) {
			upLineT[player] = 0;
			UpLineBlock(player);
			return;
		}

		PlaySE(20);

		// フィールドを上にずらす
		for(i = 1; i <= fldsizeh[player]; i++) {
			for(j = 0; j < fldsizew[player]; j++) {
				fld[j + (i - 1) * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
				fldt[j + (i - 1) * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
				fldi[j + (i - 1) * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
				flds[j + (i - 1) * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
			}
			erase[(i - 1) + player * 22] = erase[i + player * 22];
		}

		// せり上がりフィールドの読み込み
		fld[0 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2] / 10000 % 10;
		fld[1 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2] / 1000 % 10;
		fld[2 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2] / 100 % 10;
		fld[3 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2] / 10 % 10;
		fld[4 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2] % 10;
		fld[5 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2 + 1] / 10000 % 10;
		fld[6 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2 + 1] / 1000 % 10;
		fld[7 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2 + 1] / 100 % 10;
		fld[8 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2 + 1] / 10 % 10;
		fld[9 + fldsizew[player] * fldsizeh[player] + player * 220] = upFld[upLineNo[player] * 2 + 1] % 10;

		for(j = 0; j < fldsizew[player]; j++) {
			fldt[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j + fldsizew[player] * fldsizeh[player] + player * 220] != 0) * -1;
			if(hidden[player] == 8) fldt[j + fldsizeh[player] * fldsizew[player] + player * 220] = 0;
			// アイテムを増殖させない #1.60c7o6
			fldi[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
			flds[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
		}

		erase[fldsizeh[player] + player * 22] = 0;
		upLines[player]--;

		if(upLineNo[player] + 1 < upLineNoMax)
			upLineNo[player]++;
		else
			upLineNo[player] = 0;

	} else if(riseT == 2){ // 旧式#1.60c7g2
		PlaySE(20);
		h1 = hole[player];
		h2 = hole[player];
		if(IsBigStart[player]) {
			h1 = (hole[player] / 2) * 2;
			h2 = h1 + 1;
		}
		for(i = 1; i <= fldsizeh[player]; i++) {
			for(j = 0; j < fldsizew[player]; j++) {
				fld[j + (i - 1) * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
				fldt[j + (i - 1) * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
				fldi[j + (i - 1) * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
				flds[j + (i - 1) * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
			}
			erase[(i - 1) + player * 22] = erase[i + player * 22];
		}

		for(j = 0; j < fldsizew[player]; j++) {
			fld[j + fldsizew[player] * fldsizeh[player] + player * 220] = ((j != h1) && (j != h2));
			fldt[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j + fldsizew[player] * fldsizeh[player] + player * 220] != 0) * -1;
			// アイテムを増殖させない #1.60c7o6
			 fldi[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
			 flds[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
			 if(hidden[player] == 8) fldt[j + fldsizeh[player] * fldsizew[player] + player * 220] = -60;
		}

		erase[fldsizeh[player] + player * 22] = 0;
		upLines[player]--;
		// DS風に3割の確立で穴の位置が変わる C7T3.2EX
		// MISSIONでは8割の確率
		if(gameMode[player] != 4){
			if( rand(10,player) > 6 - ( (4 + (repversw >= 55) )*(gameMode[player] == 8)) ){
				do {
					newhole = rand(10,player);
				} while(newhole == hole[player]);
				hole[player] = newhole;
			}
		}else{	//対戦　NORMAL 4割　ATTACK 6割　DEFENCE&ITEM 3割
			if( rand(10,player) > 5-(2*(vs_style[1 - player] == 1))+(1*(vs_style[1 - player] > 1)) ){
				do {
					newhole = rand(10,player);
				} while(newhole == hole[player]);
				hole[player] = newhole;
			}
		}
	} else if(riseT == 4){ // 穴固定
		PlaySE(20);
		h1 = hole[player];
		h2 = hole[player];
		if(IsBigStart[player]) {
			h1 = (hole[player] / 2) * 2;
			h2 = h1 + 1;
		}
		for(i = 1; i <= fldsizeh[player]; i++) {
			for(j = 0; j < fldsizew[player]; j++) {
				fld[j + (i - 1) * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
				fldt[j + (i - 1) * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
				fldi[j + (i - 1) * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
				flds[j + (i - 1) * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
			}
			erase[(i - 1) + player * 22] = erase[i + player * 22];
		}

		for(j = 0; j < fldsizew[player]; j++) {
			fld[j + fldsizew[player] * fldsizeh[player] + player * 220] = ((j != h1) && (j != h2));
			fldt[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j + fldsizew[player] * fldsizeh[player] + player * 220] != 0) * -1;
			// アイテムを増殖させない #1.60c7o6
			 fldi[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
			 flds[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
			 if(hidden[player] == 8) fldt[j + fldsizeh[player] * fldsizew[player] + player * 220] = -60;
		}

		erase[fldsizeh[player] + player * 22] = 0;
		upLines[player]--;

	} else {
		PlaySE(20);
		// フィールドを上にずらす
		for(i = 1; i <= fldsizeh[player]; i++) {
			for(j = 0; j < fldsizew[player]; j++) {
				fld[j + (i - 1) * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
				fldt[j + (i - 1) * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
				fldi[j + (i - 1) * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
				flds[j + (i - 1) * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
			}
			erase[(i - 1) + player * 22] = erase[i + player * 22];
		}
		for(j = 0; j < fldsizew[player]; j++) {
			fldt[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j + fldsizew[player] * fldsizeh[player] + player * 220] != 0) * -1;
			// アイテムを増殖させない #1.60c7o6
			fldi[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
			flds[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
		}
		// せり上がりフィールドの一番上のラインをコピー
		for( j = 0 ; j < fldsizew[player] ; j++ ) {
			fld[j + fldsizeh[player] * fldsizew[player] + player * 220] = fldu[j + 0 * fldsizew[player] + player * 220];
			fldt[j + fldsizeh[player] * fldsizew[player] + player * 220] = (fld[j + fldsizeh[player] * fldsizew[player] + player * 220] != 0) * -1;
			if(hidden[player] == 8) fldt[j + fldsizeh[player] * fldsizew[player] + player * 220] = -60;
		}

		// せり上がりフィールドをずらす
		for(i = 1; i < 22; i++) {
			for(j = 0; j < 10; j++) {
				fldu[j + ( i - 1 ) * 10 + player * 220] = fldu[j + i * 10 + player * 220];
			}
		}
		erase[fldsizeh[player] + player * 22] = 0;
		upLines[player]--;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.06 - 時間稼ぎ & レベルアップ判定
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statErase(int player) {
	int		i, j, wkfld,tmp_s[2],y;

	padRepeat(player);

	hiddenProc(player);
//	UpLineBlockJudge(player);

	if(((gameMode[player]==1)||(gameMode[player]==2))&&(enable_grade[player] == 4)){//置く度に呼び出す必要がある
		GradeUp3(player);
	}

	if(statc[player * 10 + 3] != 0){
		if(repversw >= 57){
			drawCBlock(player, 0, 0, 0, 10, 0);
			if(spawn_y_type) viewFldFrame(1,player);
		}
		statc[player * 10 + 3]--;
	}

	if(ace_irs) doIRS2(player);

	if((repversw >= 48) && (dhold[player] != disable_hold) && (!ishidenext[player])&& (!isfakenext[player]))
		dhold[player] = disable_hold;		// hold使用可能に	#1.60c

	// ↓を離している場合、下入れ制限解除#1.60c7n6
	if((!getPressState(player, 1-(1*((isUDreverse[player]) && (repversw>=44)) ))) && (repversw >= 10)) down_flag[player] = 0;
	if(!getPressState(player, 0+(1*((isUDreverse[player]) && (repversw>=44)) ))) up_flag[player] = 0;

	if((gameMode[player] == 6) && (!ending[player])) {
		// tomoyoモードでクリアフラグが1になっていたらステージクリア #1.60c7l8
		if( sclear[player] ) {
			thunder_timer = 0;
			PlaySE(31);				// stageclear.wav
			timeOn[player] = 0;			// タイマーストップ #1.60c7m2
			clearnum[player]++;			// クリアステージ数増加 #1.60c7m5
			recStageTime(player);			//ステージクリアタイム
			if((stage[player] == 44) && (!t_training[player])){
				PlaySE(28);
				ending[player] = 1;
				stat[player] = 13;
				statc[player * 10] = 0;
				statc[player * 10 + 1] = 0;
				return;
			}
			statc[player * 10] = 0;		// カウンタを0に
			statc[player * 10 + 1] = 0;
			stat[player] = 17;			// ステージ終了
			return;
		}
		// ステージタイムが0だったらタイムオーバー
		else if(stime[player] <= 0) {
			timeOn[player] = 0;				// タイマーストップ
			sclear[player] = 0;				// タイムオーバー
			statc[player * 10] = 0;			// カウンタを0に
			statc[player * 10 + 1] = 0;
			recFaultTime(player);
			stat[player] = 17;				// ステージ終了
			return;
		}

	}
	if(((gameMode[player] == 6) || ((gameMode[player] == 8) && (mission_type[c_mission] == 6)))
	&& (statc[player * 10] == wait1[player]) && (upLines[player]))
		UpLineBlockJudge(player);

	if(ismiss[player]){
		y = Rand(8);
		ofs_x[player] = y - (8 / 2);
		ofs_x2[player] = ofs_x[player];
		if(statc[player * 10] == wait1[player])
			PlaySE(45);
	}

	if(((repversw >= 46) && ((repversw < 48))) && (statc[player * 10] == wait1[player]) && (!ishidenext[player])&& (!isfakenext[player]))
		dhold[player] = disable_hold;		// hold使用可能に	#1.60c

	if((isthunder[player]) && (statc[player * 10] == wait1[player])){
		PlaySE(37);
		thunder_timer = 30;
	}
	// 正方形判定
	if((gameMode[player]!=8)&&(squaremode[player])) {
		makeSquare(player);
	}
	if(gameMode[player]==5){
		checkEnding(player,0);
	}
	// MISSIONモードでのミッション達成
	if(gameMode[player] == 8) {
		if((ltime[player] <= 0) && (mission_type[c_mission] == 23) && (!ending[player])){ //耐久ミッション
			missionNormUp(2);
			if( (ending[player] == 1) || (ending[player] == 4) ) PlaySE(28);
			statc[player * 10 + 3] = 0;
		}
		if((mission_type[c_mission] == 29) && (!ending[player]) && (statc[player * 10] == wait1[player])){	//OOBAKA
			missionNormUp(2);
			statc[player * 10 + 3] = 0;
		}
				// 正方形判定
		if(squaremode[player]) {
			tmp_s[player] = makeSquare(player);
			if( (((mission_type[c_mission] == 40) && (tmp_s[player] >= 1)) || ((mission_type[c_mission] == 41) && (tmp_s[player] >= 2))&& (!ending[player])) ){
				missionNormUp(2);
				statc[player * 10 + 3] = 0;
			}
		}
		if( (!timeOn[player]) && (!ending[player]) ) {
			// ライン消去発生
			if(mission_erase[c_mission - 1] > 0) {
				j = mission_erase[c_mission - 1];

				for(i = checkFieldTop(player); i < 22; i++) {
					erase[i + player * 22] = 1;

					j--;
					if(j <= 0) break;
				}

				stat[player] = 25;
				statc[player * 10] = 0;
				statc[player * 10 + 1] = 3;	// Ready
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;
				if((repversw >= 41) && ((heboGB[player] > 0) || (mission_end[c_mission - 1] == 3))){
					stat[player] = 35;
					statc[player * 10] = -wait1[player];
					statc[player * 10 + 2] = mission_erase[c_mission - 1];
				}
			// ラインせり上がり穴そろい
			}else if(mission_erase[c_mission - 1] < -20) {
				upLineT[player] = 4;//実際はない
				//upLines[player] = abs_YGS2K(mission_erase[c_mission - 1]+20);
				statc[player * 10 + 3] = abs_YGS2K(mission_erase[c_mission - 1]+20);
				stat[player] = 22;
				statc[player * 10] = wait1[player] / 2;
				statc[player * 10 + 1] = 3;	// Ready
				statc[player * 10 + 4] = 4;
			} else if(mission_erase[c_mission - 1] < 0) {
				upLineT[player] = 2;
//				upLines[player] = abs_YGS2K(mission_erase[c_mission - 1]);
				statc[player * 10 + 3] = abs_YGS2K(mission_erase[c_mission - 1]);
				stat[player] = 22;
				statc[player * 10] = wait1[player] / 2;
				statc[player * 10 + 1] = 3;	// Ready
				statc[player * 10 + 4] = 2;
			} else {
				stat[player] = 22;			// 少し待つ
				statc[player * 10] = wait1[player];
				statc[player * 10 + 1] = 3;	// Ready
			}

			return;
		}
	}
	//stat[player]を変更するアイテム群の発動
	//足切り不発問題の解決と共に移動 C7U1
	//↑DEL FIELD
	if(isUPdelfield[player]){
		j = (22-checkFieldTop(player)) / 2;
		for(i = checkFieldTop(player); i <= 22; i++) {
			erase[i + player * 22] = 1;
			j--;
			if(j < 0) break;
		}
		stat[player] = 25;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	//↓DEL FIELD
	if(isDWdelfield[player]){
		j = (22-checkFieldTop(player)) / 2;
		for(i = 22; i > checkFieldTop(player); i--) {
			erase[i + player * 22] = 1;
			j--;
			if(j < 0) break;
		}
		stat[player] = 25;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	//DEL EVEN
	if(isdeleven[player]){
		for(i = 22; i >= checkFieldTop(player); i--) {
			erase[i + player * 22] = 1;
			i--;
		}
		stat[player] = 25;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	//FREE FALL
	if(isFreefall[player]){
		stat[player] = 32;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		return;
	}
	//MOV FIELD
	if((isLmovfield[player]) || (isRmovfield[player])){
		stat[player] = 33;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		return;
	}
	//ALLCLEAR
	if(isallclear[player]){
//		for(i = 0; i < 22; i++) {
//			erase[i + player * 22] = 1;
//		}
		stat[player] = 35;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		statc[player * 10] = -wait1[player];
		statc[player * 10 + 2] = 21;
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	if(doshotgun[player]){	//ショットガン発動
		stat[player] = 28;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		doshotgun[player] = 0;
		return;
	}
	if(dolaser[player]){	//レーザー発動
		stat[player] = 26;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		dolaser[player] = 0;
		return;
	}
	if(donega[player]){	//ネガフィールド発動
		stat[player] = 27;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		donega[player] = 0;
		return;
	}
	if(dorulet[player]){	//アイテムルーレット発動
		stat[player] = 31;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		dorulet[player] = 0;
		return;
	}
	if(do180field[player]){	//180°フィールド発動
		stat[player] = 34;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		do180field[player] = 0;
		return;
	}
	if(doexchg[player]){	//フィールド交換発動
		stat[player] = 29;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		doexchg[player] = 0;
		return;
	}if(do16t[player]){	//16t発動
		stat[player] = 26;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 1;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		do16t[player] = 0;
		return;
	}
	if(docopyfld[player]){	//フィールドコピー発動
		stat[player] = 29;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 1;
		statc[player * 10 + 4] = 0;
		docopyfld[player] = 0;
		return;
	}

	statc[player * 10]--;


	//D.R.SのAREスキップ
	if( ((rots[player] == 8) && (harddrop[player])) &&
	((getPushState(player, 0)) || (getPushState(player, 1)) || (getPushState(player, 2)) ||
	 (getPushState(player, 3)) || (getPushState(player, 4)) || (getPushState(player, 5)) ||
	 (getPushState(player, 6)) || (getPushState(player, 7))) )
		statc[player * 10] = -1;

	if( ((rots[player] == 8) && (are_skipflag[player])) &&
	((getPressState(player, 0)) || (getPressState(player, 1)) || (getPressState(player, 2)) ||
	 (getPressState(player, 3)) || (getPressState(player, 4)) || (getPressState(player, 5)) ||
	 (getPressState(player, 6)) || (getPressState(player, 7))) ){
	 	are_skipflag[player] = 0;
		statc[player * 10] = -1;
	}

	if((statc[player * 10] < wait1[player] - 1) && (wait1[player] != 0) &&
		(cpu_flag[player]) && (rots[player] != 6) && (rots[player] != 8)){
		for(i=0;i<10;i++) {
			cp_input[i + player * 10] = 0;
		}
	}

	if( (statc[player * 10] < 0) || (rots[player] == 6) ) {

		ofs_x[player] = 0;
		ofs_x2[player] = 0;

		//エンディング突入よりも優先するように
		//（機能しなくなるから）C7U1
		// せりあがり処理（ミラー中はせり上げない）
		if(fmirror_cnt[player] == -20+(20*(repversw <= 17)))
			if(UpLineShirase(player)) return;

		// フィールドミラー化処理 #1.60c7j5
		if(fldMirrorProc(player)) return;

		if(ending[player]) {
			// エンディング突入
			stat[player] = 13;

			// ステータスカウンタ初期化 #1.60c7r3
			if(repversw >= 15) {
				for(i = 0; i < 10; i++) statc[i + player * 10] = 0;
			}
			if((gameMode[player] == 9) && (relaymode[player])){
				end_f[player] = 0;
				ending[player] = 0;
				stat[player] = 15;
				statc[player * 10 + 0] = 0;
				statc[player * 10 + 1] = 0;
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;	//クリア演出
				return;
			}

		} else {
			// レベルアップ
			LevelUp(player);

			// このバージョンから固定→次を2フレーム縮める #1.60c7h2
			if(repversw < 4)
				stat[player] = statc[player * 10 + 1];
			else {
				// REメダル
				if( pinch[player] ) {
					REflag[player] = 1;
				} else if( (REflag[player]) && (pinch2[player]) && (!ending[player]) &&(medaltype <= 1)&&(gameMode[player] != 0)
					&&(gameMode[player] != 4) && (gameMode[player] <= 5) ) {
					rescue[player]++;//この回数を画面に表示し、メダルを決める
					REflag[player] = 0;

						if( ( (rescue[player] == 2) && (medal_re[player] == 0) )
						 || ( (rescue[player] == 4) && (medal_re[player] == 1) )
						 || ( (rescue[player] == 6) && (medal_re[player] == 2) )
						|| ( (rescue[player] == 8) && (medal_re[player] == 3) ))
						{
							PlaySE(39);
							medal_re[player]++;
							objectCreate2(player, 11,208 + 81 * player - 96 * maxPlay, 148,3,0,0,medal_re[player] - 1);
						}
				}
				if( (gameMode[player] == 4) || (item_mode[player]) || (gameMode[player] == 8) ) {
					// ミッションモードでも使用 #1.60c7s5

					// 対戦用ロールロール #1.60c7n1
					if( item_rollroll[player] ) {
						isrollroll[player] = 1;
					}

					// 対戦用X-RAY #1.60c7r3
					if( xray_flag[player] ) {
						isxray[player] = 1;
					}
				} else {
					// ロールロール化判断 #1.60c7j5
					isrollroll[player] = 0;
					if((rollroll_flag[player]) && (p_rollroll_interval > 0) && (gameMode[player] >= 5))
						if((bdowncnt[player] + 1) % p_rollroll_interval == 0)
							isrollroll[player] = 1;

					// X-RAY判断 #1.60c7p9ex
					isxray[player] = 0;
					if((xray_flag[player]) && (p_xray_interval > 0) && (gameMode[player] >= 5))
						if((bdowncnt[player] + 1) % p_xray_interval == 0)
						{
							isxray[player] = 1;
							xray_counter[player] = 0;
						}
				}
				// ステータスカウンタ初期化 #1.60c7r3
				if(repversw >= 61)
					for(i = 0; i < 10; i++) statc[i + player * 10] = 0;
				// 落下開始
				ofs_x[player] = 0;
				ofs_x2[player] = 0;
				statBlock(player);
			}
			if(repversw < 61)
				for(i = 0; i < 10; i++) statc[i + player * 10] = 0;
		}

	}
}

/* フィールドミラー */
int fldMirrorProc(int player) {
	int		i, j, wkfld;
	if((stopmirror_flag[player] == 1) && (fmirror_cnt[player] == -20)){
		isfmirror[player] = 0;
		stopmirror_flag[player] = 0;
		if((stat[player] == 13) && ((ending[player] == 3) || (ending[player] == 7)))
			disableShadowTimer(player);
		return 0;
	}


	if((repversw <= 17) && (fmirror_cnt[player] < 0))	//c7t2.9以前のリプレイはフィールド回転無し
		fmirror_cnt[player] = 0;

	// fmirrorをplayerごとに #1.60c7l7
	if((isfmirror[player]) && (p_fmirror_interval > 0) && ((gameMode[player] >= 4) || (item_mode[player]))) {
		if( (((gameMode[player] == 4) || (item_mode[player]))&&(bdowncnt[player] % 1 == 0)) || (bdowncnt[player] % p_fmirror_interval == 0) || (gameMode[player] == 8) ) {
			if(fmirror_cnt[player] <= p_fmirror_timer) {
				if(((repversw >= 18) && (fmirror_cnt[player] == -20)) || ((repversw <= 17) && (fmirror_cnt[player] == 0))) {
//					PlayWave(20);
					// フィールドをバッファに確保
					for(i = 0; i <= fldsizeh[player]; i++) {
						for(j = 0; j < fldsizew[player]; j++) {
								fldbuf[j + i * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
							if(repversw >= 18)	//旧式はフィールドを消さない
								fld[j + i * fldsizew[player] + player * 220] = 0;
								fldtbuf[j + i * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
							if(repversw >= 18)	//旧式はフィールドを消さない
								fldt[j + i * fldsizew[player] + player * 220] = 0;
							if((gameMode[player] == 4) || (item_mode[player])){
								fldibuf[j + i * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
								fldi[j + i * fldsizew[player] + player * 220] = 0;
							}
							fldsbuf[j + i * 10 + player * 220] = flds[j + i * 10 + player * 220];
							flds[j + i * 10 + player * 220] = 0;
						}
					}
				} else {
					if((fmirror_cnt[player] > 0) && (fmirror_cnt[player] % (p_fmirror_timer / fldsizew[player]) == 0)) {
						for(i = 0; i <= fldsizeh[player]; i++) {
							j = (fmirror_cnt[player]) / (p_fmirror_timer / fldsizew[player]) - 1;
							fld[(fldsizew[player] - j - 1) + i * fldsizew[player] + player * 220] = fldbuf[j + i * fldsizew[player] + player * 220];
							fldt[(fldsizew[player] - j - 1) + i * fldsizew[player] + player * 220] = fldtbuf[j + i * fldsizew[player] + player * 220];
							flds[(fldsizew[player] - j - 1) + i * fldsizew[player] + player * 220] = fldsbuf[j + i * fldsizew[player] + player * 220];
							if((gameMode[player] == 4) || (item_mode[player]))
								fldi[(fldsizew[player] - j - 1) + i * fldsizew[player] + player * 220] = fldibuf[j + i * fldsizew[player] + player * 220];
						}
					ExBltFast(27, 120+ ((fldsizew[player] - j - 1) * 8) + 192 * player - 96 * maxPlay, 40);
					}
				}
				fmirror_cnt[player]++;
				if((fastmirror) && (fmirror_cnt[player] <= 0)) fmirror_cnt[player]++;
				return 1;
			} else if((repversw >= 18) && (fmirror_cnt[player] <= p_fmirror_timer + 10-(6*(repversw >= 25)) + 10*(fastmirror))) {
				fmirror_cnt[player]++;
				return 1;
			}
			fmirror_cnt[player] = -20;
			if(repversw <= 17)	//c7t2.9以前のリプレイはフィールド回転無し
			fmirror_cnt[player] = 0;
		}
	}
	return 0;
}

/* せり上がり */
int UpLineShirase(int player) {

	// 旧式せり上がり（リプレイの互換性のため残されている）
	if(repversw < 6) {
		if((p_shirase[player] == 1) && (tc[player] >= shirase[player]) && (tc[player] >= level_shirase_start + raise_shirase_interval) ) {
			shirase[player] = (tc[player] / raise_shirase_interval + 1) * raise_shirase_interval;
			if(tc[player] < shirase[player])
				upLines[player] = raise_shirase_lines;
			else
				upLines[player] = 0;
		}

		if((gameMode[player] == 5) && (p_shirase[player] == 1) && (tc[player] > shirase[player])) {
			upLines[player] = raise_shirase_lines * ((IsBig[player] && big_rise) + 1);
			shirase[player] = shirase[player] + raise_shirase_interval;
		}
	// DEVILランダムせり上がり C7U4.9
	} else if((gameMode[player] == 3) && (devil_randrise) && (repversw >= 44) && (!devil_minus[player])){
		if((devil_nextrise[player] <= 0) && (devil_rise_min[tr[player] / 10] > 0) && (devil_rise_max[tr[player] / 10] > 0)) {
			do {
				devil_nextrise[player] = rand( devil_rise_max[tr[player] / 10] + 1,player);
			} while(devil_nextrise[player] < devil_rise_min[tr[player] / 10]);

			upLines[player] = 1;
		}
	// このバージョンからせり上がりの仕様を変更 #1.60c7i2
	} else {
		// DEVILのTIME TRIALのLV1000以降でせり上がらないようにした #1.60c7i2
		if((gameMode[player] == 3) && (tc[player] >= 1000))
			upLines[player] = 0;
		// MISSION DEVIL800
		else if((gameMode[player] == 8) && (mission_type[c_mission] == 19) && (st_bdowncnt[player] >= shirase[player])) {
			shirase[player] = (st_bdowncnt[player] / mission_opt_3[c_mission] + 1) * mission_opt_3[c_mission];
			if(st_bdowncnt[player] < shirase[player])
				upLines[player] = 1;
			else
				upLines[player] = 0;
		}

		// せり上がり間隔をレベルではなく、ミノを着床させた回数に変更 #1.60c7i2
		else if((p_shirase[player] == 1) && (bdowncnt[player] >= shirase[player]) && (tc[player] >= level_shirase_start)) {
			shirase[player] = (bdowncnt[player] / raise_shirase_interval + 1) * raise_shirase_interval;
			if(bdowncnt[player] < shirase[player])
				upLines[player] = raise_shirase_lines * ((IsBig[player] && big_rise) + 1);
			else
				upLines[player] = 0;
		}
	}

	UpLineBlockJudge(player);
	if(upLines[player]) return 1;

	if(repversw >= 47) pinchCheck(player);

	return 0;
}

/* レベルアップ */
void LevelUp(int player) {
	// レベルアップ判定にTGMルール追加 #1.60c3
	if(gameMode[player] == 0) {
		// ビギナーモードのレベルアップ判定
		if(tc[player] / 10 > tr[player]) {
			tr[player]++;
			recSectionTime(player);

			// スタッフロールでないならスピード値とwait値を変える #1.60c7j8
			if(!ending[player]) {
				if(tr[player] < 101){
					if(repversw < 40)sp[player] = lvTableBeg39[tr[player]];
					else sp[player] = lvTableBeg[tr[player]];
				}
				if(tr[player] % 10 == 0) {
					PlaySE(19);
					bgfadesw = 1;
					if(fadelv[player] != 0) {
						bgmlv++;
						changeBGM(player);
					}
				}
			}
		}
	} else if((gameMode[player] == 1)||(gameMode[player] == 2)) {
		// マスターモード,20Gのレベルアップ判定
		if(((enable_grade[player] != 4)||(repversw<=23))) {
			if(tc[player] / 10 > tr[player]) {
				tr[player]++;
				recSectionTime(player);
				if(!ending[player]) {
					if(repversw <= 20){//リプレイが20以下
						if(tr[player] < 51) sp[player] = lvTableTgm20[tr[player]];
					}else if(repversw > 20){
						if(tr[player] < 51){
							if(gameMode[player] == 2){
							//20G
								sp[player] = 1200;
							}else{
							//MASTER
							 	sp[player] = lvTableTgm[tr[player]];//前半は10ごとに上がる
							}
						}
					}
					if(tr[player] % 10 == 0) {
						if(((enable_grade[player] != 1)||(repversw<=23))){
							if(repversw<29){//28まで
								if((tr[player] >= 50) && (tr[player] <= 100)) {
									wait1[player] = wait1_master_tbl28[(tr[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl28[(tr[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl28[(tr[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl28[(tr[player] - 50) / 10 * 2];
								}
							}else if((repversw>=29) && (repversw<35)){//34まで
								if((tr[player] >= 50) && (tr[player] <= 100)) {
									wait1[player] = wait1_master_tbl34[(tr[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl34[(tr[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl34[(tr[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl34[(tr[player] - 50) / 10 * 2];
								}
							}else if((repversw>=35) && (repversw<54)){
								if((tr[player] >= 50) && (tr[player] <= 100)) {
									wait1[player] = wait1_master_tbl53[(tr[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl53[(tr[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl53[(tr[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl53[(tr[player] - 50) / 10 * 2];
								}
							}else{
								if((tr[player] >= 50) && (tr[player] <= 100)) {
									wait1[player] = wait1_master_tbl[(tr[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl[(tr[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl[(tr[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl[(tr[player] - 50) / 10 * 2];
								}
							}
						}else if(((enable_grade[player] == 1)&&(repversw>24))||((hebo_plus[player])&&(repversw>=34))){
							if((tr[player] >= 50) && (tr[player] <= 100)) {
							sp[player] = 1200;
							wait1[player] =	26;
							wait2[player] = 40;
							wait3[player] = 28;
							waitt[player] = 15;
							}
						}
					PlaySE(19);
					bgfadesw = 1;
					hebop_interval[player]--;
						if((fadelv[player] != 0) && (!ending[player])) {
							bgmlv++;
							changeBGM(player);
						}
					}
				}
			}
		} if((enable_grade[player] == 4)&&(repversw >= 24)) {//grade4用速度アップ用
			if(tc[player] / 10 > tr[player]) {//tcはレベル trは10ごとに1ずつ増える
				tr[player]++;
				tr2[player]++;
				recSectionTime(player);

				if(!ending[player]) {

					if(tr[player] % 10 == 0) {//100ごと
						isregret(player);//regret判定
						if(gup3sp_adjust[player] == 2){
							tr2[player] = tr2[player] + 10;//skip
						}
						if((gup3sp_adjust[player] == 0) && ((regretcancel[player] == 0) || (repversw < 41))){
							tr2[player] = tr2[player] - 10;//やり直し
						}
					}
					if(tr2[player] < 51){
						if(gameMode[player] == 2){
						//20G
							sp[player] = 1200;
						}else{
						//MASTER
						 	sp[player] = lvTableTgm[tr2[player]];//前半は10ごとに上がる
						}
					}
					if(tr[player] % 10 == 0) {
						if((tr2[player] >= 50) && (tr2[player] <= 200) && ((tr2[player] <= 150) || (repversw < 60))) {
							if(repversw<29){//29以前
								if((tr2[player] >= 50) && (tr2[player] <= 200)) {
									sp[player] = 1200;
									wait1[player] = wait1_master_tbl28[(tr2[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl28[(tr2[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl28[(tr2[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl28[(tr2[player] - 50) / 10 * 2];
								}
							} else if((repversw >=29) && (repversw < 35)){//29以降
								if((tr2[player] >= 50) && (tr2[player] <= 200)) {
									sp[player] = 1200;
									wait1[player] = wait1_master_tbl34[(tr2[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl34[(tr2[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl34[(tr2[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl34[(tr2[player] - 50) / 10 * 2];
								}
							} else if((repversw >=35) && (repversw < 54)){//35以降
								if((tr2[player] >= 50) && (tr2[player] <= 200)) {
									sp[player] = 1200;
									wait1[player] = wait1_master_tbl53[(tr2[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl53[(tr2[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl53[(tr2[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl53[(tr2[player] - 50) / 10 * 2];
								}
							} else if(repversw >=54){//54以降
								if((tr2[player] >= 50) && (tr2[player] <= 200)) {
									sp[player] = 1200;
									wait1[player] = wait1_master_tbl[(tr2[player] - 50) / 10 * 2];
									wait2[player] = wait2_master_tbl[(tr2[player] - 50) / 10 * 2];
									wait3[player] = wait3_master_tbl[(tr2[player] - 50) / 10 * 2];
									waitt[player] = waitt_master_tbl[(tr2[player] - 50) / 10 * 2];
								}
							}
						}
						PlaySE(19);
						reset_gradeup3(player);//gradeup3に使う変数を一気にリセット
						bgfadesw = 1;
						hebop_interval[player]--;
						if((fadelv[player] != 0) && (!ending[player])) {
							bgmlv++;
							changeBGM(player);
						}
					}
				}
			}
		}
	} else if(gameMode[player] == 3) {
		// デビルモードのレベルアップ判定
		if(repversw < 18){//17以前
			if(tc[player] / 10 > tr[player]) {
				tr[player]++;
				recSectionTime(player);

				if( (!ending[player]) && (tr[player] % 10 == 0) ) {
					if(tr[player] <= 120) {	// LV1200まで #1.60c7j5
						wait1[player] = wait1_devil_tbl17[tr[player] / 10];
						wait2[player] = wait2_devil_tbl17[tr[player] / 10];
						wait3[player] = wait3_devil_tbl17[tr[player] / 10];
						waitt[player] = waitt_devil_tbl17[tr[player] / 10];

						if(grade[player] < 13)
							grade[player] = tr[player] / 10; // Grade設定 #1.60c7j7
							gflash[player]=120;
					}
					PlaySE(19);
					bgfadesw = 1;
					if((fadelv[player] != 0) && (!ending[player])) {
						bgmlv++;
						changeBGM(player);
					}
				}
			}
		}else if(repversw == 18){//18-19
			if(tc[player] / 10 > tr[player]) {
				tr[player]++;
				recSectionTime(player);

				if( (!ending[player]) && (tr[player] % 10 == 0) ) {
					if(tr[player] <= 120) {	// LV1200まで #1.60c7j5
						wait1[player] = wait1_devil_tbl18[tr[player] / 10];
						wait2[player] = wait2_devil_tbl18[tr[player] / 10];
						wait3[player] = wait3_devil_tbl18[tr[player] / 10];
						waitt[player] = waitt_devil_tbl18[tr[player] / 10];

						if(grade[player] < 13)
							grade[player] = tr[player] / 10; // Grade設定 #1.60c7j7
							gflash[player]=120;
					}
					PlaySE(19);
					bgfadesw = 1;
					if((fadelv[player] != 0) && (!ending[player])) {
						bgmlv++;
						changeBGM(player);
					}
				}
			}
		}else if(repversw >= 19){//19以降
			if(tc[player] / 10 > tr[player]) {
				tr[player]++;
				recSectionTime(player);

				if( (!ending[player]) && (tr[player] % 10 == 0) ) {
					if((((enable_grade[player] == 1) && (repversw < 42)) || (devil_minus[player]))&&(gameMode[player] == 3)&&(repversw >= 31)){//devil-
						if(tr[player] <= 120) {	// LV1200まで #1.60c7j5
							wait1[player] = wait1_devil_m_tbl[tr[player] / 10];
							wait2[player] = wait2_devil_m_tbl[tr[player] / 10];
							wait3[player] = wait3_devil_m_tbl[tr[player] / 10];
							waitt[player] = waitt_devil_m_tbl[tr[player] / 10];

							if(!devil_minus[player]){
								if(grade[player] < 13){//ノーマル
									grade[player] = tr[player] / 10; // Grade設定 #1.60c7j7
									gflash[player]=120;
								}
							}else{
								if(tr[player]==50){//m
									grade[player]=14;
									PlaySE(28);
									gflash[player]=120;
								}
							}
						}
					}else if(repversw < 33){	//32
						if(tr[player] <= 120) {	// LV1200まで #1.60c7j5
							wait1[player] = wait1_devil_tbl32[tr[player] / 10];
							wait2[player] = wait2_devil_tbl32[tr[player] / 10];
							wait3[player] = wait3_devil_tbl32[tr[player] / 10];
							waitt[player] = waitt_devil_tbl32[tr[player] / 10];

							if(grade[player] < 13){
								grade[player] = tr[player] / 10; // Grade設定 #1.60c7j7
								gflash[player]=120;
							}
						}
					}else{
						if(tr[player] <= 120) {	// LV1200まで #1.60c7j5
							if(repversw < 60){
								wait1[player] = wait1_devil_tbl[tr[player] / 10];
								wait2[player] = wait2_devil_tbl[tr[player] / 10];
								wait3[player] = wait3_devil_tbl[tr[player] / 10];
								waitt[player] = waitt_devil_tbl[tr[player] / 10];
							}else if(repversw < 62){
								wait1[player] = wait1_doom_tbl61[tr[player] / 10];
								wait2[player] = wait2_doom_tbl61[tr[player] / 10];
								wait3[player] = wait3_doom_tbl61[tr[player] / 10];
								waitt[player] = waitt_doom_tbl61[tr[player] / 10];
							}else{
								wait1[player] = wait1_doom_tbl[tr[player] / 10];
								wait2[player] = wait2_doom_tbl[tr[player] / 10];
								wait3[player] = wait3_doom_tbl[tr[player] / 10];
								waitt[player] = waitt_doom_tbl[tr[player] / 10];
							}
							if((!devil_minus[player]) && (!death_plus[player])){
								if(grade[player] < 13)//ノーマル
								grade[player] = tr[player] / 10; // Grade設定 #1.60c7j7
								gflash[player]=120;
							}else{
								if(tr[player]==50){
									PlaySE(28);
									grade[player]=14;
									gflash[player]=120;
								}
								if(tr[player]==100){
									PlaySE(28);
									grade[player]=15;
									gflash[player]=120;
								}
								if(tr[player]==130){
									PlaySE(28);
									grade[player]=16;
									gflash[player]=120;
								}
							}
						}
					}
					PlaySE(19);
					bgfadesw = 1;
					if((devil_randrise) && (repversw >= 44)){
						if((devil_nextrise[player] <= 0) && (devil_rise_min[tr[player] / 10] > 0) && (devil_rise_max[tr[player] / 10] > 0) && (!devil_minus[player])) {
							do {
								devil_nextrise[player] = rand( devil_rise_max[tr[player] / 10] + 1,player);
							} while(devil_nextrise[player] < devil_rise_min[tr[player] / 10]);
						}
					}
					if((fadelv[player] != 0) && (!ending[player])) {
						bgmlv++;
						changeBGM(player);
					}
				}
			}
		}
	} else if(gameMode[player] == 4) {
		// VSモードのレベルアップ判定 #1.60c7r4
		if(tc[player] / 10 > tr[player]) {
			tr[player]++;

			if( (!ending[player]) && (tr[player] % 10 == 0) ) {
				PlaySE(19);
			}

		}
		if(wintype==0){
			// ゴールに到達
			if( (vs_goal != 0) && (tc[player] >= vs_goal) ) {
				tc[player] = vs_goal;
				PlaySE(19);

				// 相手をゲームオーバーにさせる
				stat[1 - player] = 7;
				statc[0 + (1 - player) * 10] = 0;
				statc[1 + (1 - player) * 10] = 0;
				statc[2 + (1 - player) * 10] = 0;
				statc[3 + (1 - player) * 10] = 0;
				statc[4 + (1 - player) * 10] = 0;
				statc[5 + (1 - player) * 10] = 0;
				if(istimestop[1-player]) istimestop[1-player] = 0;
			}
		}else if(wintype==1){
			// ゴールに到達
			if( (vs_goal != 0) && (li[player] >= vs_goal/10) ) {
				li[player] = vs_goal/10;
				PlaySE(30);
				// 相手をゲームオーバーにさせる
				stat[1 - player] = 7;
				statc[0 + (1 - player) * 10] = 0;
				statc[1 + (1 - player) * 10] = 0;
				statc[2 + (1 - player) * 10] = 0;
				statc[3 + (1 - player) * 10] = 0;
				statc[4 + (1 - player) * 10] = 0;
				statc[5 + (1 - player) * 10] = 0;
				if(istimestop[1-player]) istimestop[1-player] = 0;
			}
		}
	} else if(gameMode[player] == 6) {
		// TOMOYOモードのレベルアップ判定 #1.60c7m1
		if(!fpbas_mode[player]) {
			if(tc[player] / 10 > tr[player]) {
				tr[player]++;

				if(tr[player] < 31){
					if((repversw < 25) || ((stage[player] >= 100) && (lvTabletomoyo24[tr[player]] > sp[player])))
						sp[player] = lvTabletomoyo24[tr[player]];
					else if(lvTabletomoyo[tr[player]] > sp[player])
						sp[player] = lvTabletomoyo[tr[player]];
				}

				if(tr[player] % 10 == 0) {
					PlaySE(19);
					bgfadesw = 1;
				}
			}
		} else {
			if(lc[player] > 100) {
				lc[player] = lc[player] - 100;
				lv[player]++;

				if(lv[player] < 20){
					if((repversw < 25) || ((stage[player] >= 100) && (lvTabletomoyo24[lv[player]] > sp[player])))
						sp[player] = lvTabletomoyohebo[lv[player]];
					else if(lvTabletomoyo[lv[player]] > sp[player])
						sp[player] = lvTabletomoyohebo[lv[player]];
				}

				if(lv[player] % 2 == 0) {
					PlaySE(19);
					bgfadesw = 1;
				}
			}
		}
	} else if(gameMode[player] == 7) {
		// ACEモードのレベルアップ判定
		if( (li[player] >= lv[player] * 10) && (!ending[player]) && ( anothermode[player] == 0)) {
			lv[player]++;
			if(repversw < 56)
				sp[player] = lvTableAce55[lv[player]-1];
			else
				sp[player] = lvTableAce[lv[player]-1];
			ltime[player] = timeLimitAce[lv[player]-1];

			PlaySE(30);
			StopSE(32);
			bgfadesw = 1;
			timeOn[player] = 0;		// タイムを一時的に止める

			if(fadelv[player] != 0) {
				ace_bgmchange[player]++;
				bgmlv = ace_bgmlist[ace_bgmchange[player]];
				changeBGM(player);
			}
		} else if( (li[player] >= lv[player] * 10) && (!ending[player]) && ( anothermode[player] == 1)) {
			// アナザー
			lv[player]++;
			sp[player] = lvTableAcea[lv[player]-1];
			ltime[player] = timeLimitAcea[lv[player]-1];
			if(repversw<36){
				wait1[player] = wait1_Acea_tbl35[lv[player] - 1];
				wait2[player] = wait2_Acea_tbl35[lv[player] - 1];
				wait3[player] = wait3_Acea_tbl35[lv[player] - 1];
				waitt[player] = waitt_Acea_tbl35[lv[player] - 1];
			} else if(repversw>=36){
				wait1[player] = wait1_Acea_tbl[lv[player] - 1];
				wait2[player] = wait2_Acea_tbl[lv[player] - 1];
				wait3[player] = wait3_Acea_tbl[lv[player] - 1];
				waitt[player] = waitt_Acea_tbl[lv[player] - 1];
			}

			PlaySE(30);
			StopSE(32);
			bgfadesw = 1;

			if(fadelv[player] != 0) {
				ace_bgmchange[player]++;
				bgmlv = ace_bgmlist[ace_bgmchange[player] + 3];
				changeBGM(player);
			}
		} else if( (li[player] >= lv[player] * 10) && (!ending[player]) && ( anothermode[player] == 2)) {
			// アナザー2
			lv[player]++;
			if(lv[player] >= 16) over1000_block[player] = 1;	//レベル16以降は[]
			if((lv[player] >= 6) && (lv[player] < 8)) hidden[player] = 4;
			else if((lv[player] >= 8) && (lv[player] < 16)) hidden[player] = 5;
			else hidden[player] = 0;
			ltime[player] = timeLimitAcea2[lv[player]-1];

			PlaySE(30);
			StopSE(32);
			if((lv[player] >= 11) || (lv[player]%2==1))
			bgfadesw = 1;

			if(fadelv[player] != 0) {
				ace_bgmchange[player]++;
				bgmlv = ace_bgmlist[ace_bgmchange[player] + 6];
				changeBGM(player);
			}
		} else if( (lc[player] > 100) && (!ending[player]) && ( anothermode[player] == 3)) {
			lc[player] = 0;
			// アナザー3
			lv[player]++;
			if(heboGB[player] == 2){
				if(lv[player]<19){
					sp[player] = lvTabletomoyohebo[lv[player]-1];
				}
			}else{
				if(lv[player]<=15){
					sp[player] = lvTableHeboGB[lv[player]-1];
					wait3[player] = wait3_HeboGB_tbl[lv[player]-1];
					waitt[player] = waitt_HeboGB_tbl[lv[player]-1];
				}
			}

			PlaySE(30);
			StopSE(32);
			bgfadesw = 1;
			if(fadelv[player] != 0) {
				ace_bgmchange[player]++;
				bgmlv = ace_bgmlist[ace_bgmchange[player] + 9];
				changeBGM(player);
			}
		}
	} else if(gameMode[player] == 9){
		if( (c_norm[player] >= lv[player] * 10) && (!ending[player]) && ( std_opt[player] == 2)) {
			lv[player]++;
			c_norm[player] = c_norm[player]-((lv[player]-1) * 10);
			if(repversw < 56)
				sp[player] = lvTableAce55[lv[player]-1];
			else
				sp[player] = lvTableAce[lv[player]-1];

			PlaySE(30);
			StopSE(32);
			bgfadesw = 1;
			timeOn[player] = 0;		// タイムを一時的に止める

			if(fadelv[player] != 0) {
				ace_bgmchange[player]++;
				bgmlv = ace_bgmlist[ace_bgmchange[player]];
				changeBGM(player);
			}
		} else if(( c_norm[player] >= lv[player] * 5) && (!ending[player]) && ( std_opt[player] == 3)) {
			//
			lv[player]++;
			c_norm[player] = c_norm[player]-((lv[player]-1) * 5);
			if(repversw < 56)
				sp[player] = lvTableAce55[lv[player]-1];
			else
				sp[player] = lvTableAce[lv[player]-1];

			PlaySE(30);
			StopSE(32);
			bgfadesw = 1;

			if(fadelv[player] != 0) {
				ace_bgmchange[player]++;
				bgmlv = ace_bgmlist[ace_bgmchange[player] + 3];
				changeBGM(player);
			}
		}
	} else if(gameMode[player]==10){
		if(lc[player] > 100) {
			lc[player] = lc[player] - 100;
			lv[player]++;
			recSectionTime(player);
			if(ori_opt[player]==0) {
				if(!ending[player]) {
					if(lv[player] < 101) sp[player] = lvTableBeg39[lv[player]];

					if(lv[player] % 10 == 0) {
						PlaySE(19);
						bgfadesw = 1;
						if(fadelv[player] != 0) {
							bgmlv++;
							changeBGM(player);
						}
					}
				}
			}else if((ori_opt[player]==1)||(ori_opt[player]==2)){
				if(!ending[player]) {
					if(lv[player] < 31) {
						sp[player] = orimasterlvTable[lv[player]];
					}
					if(ori_opt[player]==2)
						sp[player] = 1200;
					if(lv[player] % 6 == 0) {
						PlaySE(19);
						if((lv[player] >= 30) && (lv[player] <= 60)) {
							wait1[player] = wait1_orimaster_tbl[(lv[player] - 30) / 3];
							wait2[player] = wait2_orimaster_tbl[(lv[player] - 30) / 3];
							wait3[player] = wait3_orimaster_tbl[(lv[player] - 30) / 3];
							waitt[player] = waitt_orimaster_tbl[(lv[player] - 30) / 3];
						}
						bgfadesw = 1;
						if((fadelv[player] != 0) && (!ending[player])) {
							bgmlv++;
							changeBGM(player);
						}
					}
				}
			}else if(ori_opt[player]==3){
			sp[player] = 1200;
				if( (!ending[player]) && (lv[player] % 3 == 0) ) {
					PlaySE(19);
					if(lv[player] <= 30) {		// LV40まで #1.60c7j5 LV30まで #1.60c7j7
						wait1[player] = wait1_oridevil_tbl[lv[player] / 3];
						wait2[player] = wait2_oridevil_tbl[lv[player] / 3];
						wait3[player] = wait3_oridevil_tbl[lv[player] / 3];
						waitt[player] = waitt_oridevil_tbl[lv[player] / 3];

						if(grade[player] < 13)
							grade[player] = lv[player] / 3; // Grade設定 #1.60c7j7
					}
					bgfadesw = 1;
						if((fadelv[player] != 0) && (!ending[player])) {
							bgmlv++;
							changeBGM(player);
						}
//					PlayWave(30);	// rankup.wav
				}
			}
		}
	} else{
		// 他のモードではレベルアップ音だけ鳴らす
		if(tc[player] / 10 > tr[player]) {
			tr[player]++;
			recSectionTime(player);

			if( (!ending[player]) && (tr[player] % 10 == 0) ) {
				PlaySE(19);
			}
		}
	}
}

/* ラップタイムとスプリットタイム記録（#1.60c7j7 ～） */
//LevelUpから独立 #1.60c7j8
void recSectionTime(int player) {
	int tmp, max,temp;

	tmp = 0;

//	if(item_mode[player]||hebo_plus[player])return;//アイテムかヘボ＋だったら記録しない
	// TGM
 	if(gameMode[player] !=6) {
		if((tr[player] % st_record_interval_tgm == 0) && (tr[player] <= 200) && (tc[player] != 0)) {
			split_time[tr[player] / st_record_interval_tgm - 1 + player * 100] = time[player];
			if(tr[player] / st_record_interval_tgm - 2 < 0) {
				lap_time[tr[player]/st_record_interval_tgm -1 +player*100] = split_time[tr[player]/st_record_interval_tgm -1 +player*100];
			} else {
				lap_time[tr[player]/st_record_interval_tgm -1 +player*200] =
				split_time[tr[player]/st_record_interval_tgm -1 +player*100] - split_time[tr[player]/st_record_interval_tgm -2 +player*100];
			}

			if(st_record_interval_tgm==10){
				//レベルストップ
				lvstop_time[tr[player]/st_record_interval_tgm -1 +player*20] = time99[player];
				//その他
				st_other[tr[player]/st_record_interval_tgm -1 +player*30] = st_bdowncnt[player];
			}
			st_bdowncnt[player] = 0;
			time99[player] = 0;

			// STメダル獲得 #1.60c7p7
			if( (gameMode[player] == 1) || (gameMode[player] == 2) )
				max = 10;
			else
				max = 13;

			if(tr[player] <= max*10) {
				tmp = ST_RankingCheck( player, gameMode[player], (tr[player]/st_record_interval_tgm -1), enable_grade[player] );
				if(tmp == 4){
					stp[player]++;
				} else if(tmp == 3) {
					stg[player]++;
					stp_point[player] = stp_point[player] + 6;
					st_new[(tr[player]/st_record_interval_tgm -1) + player * 20] = 5;
				} else if(tmp == 2) {
					sts[player]++;
					stp_point[player] = stp_point[player] + 2;
					st_new[(tr[player]/st_record_interval_tgm -1) + player * 20] = 6;
				} else if(tmp == 1) {
					stb[player]++;
					stp_point[player] = stp_point[player] + 1;
					st_new[(tr[player]/st_record_interval_tgm -1) + player * 20] = 2;
				}
				if(stp_point[player] >= 20)tmp = 4;
			}
		}
	}

	if((medaltype != 2)&&(gameMode[player] != 0) &&(gameMode[player] != 4) && (gameMode[player] <= 5) ) {
		if(medaltype==0){
			if(tmp > 0){//セクションごとにつく
				if( tmp > medal_st[player] ) {
					medal_st[player] = tmp;
				}
				PlaySE(39);
				objectCreate2(player, 11,233 + 31 * player - 96 * maxPlay, 122,1,0,0,tmp - 1);
			}
		}else if(medaltype==1){
			if( tmp > medal_st[player] ) {//色を更新しないとつかない
				medal_st[player] = tmp;
				PlaySE(39);
				objectCreate2(player, 11,233 + 31 * player - 96 * maxPlay, 122,1,0,0,medal_st[player] - 1);
			}
		}
	}
}
/* ステージクリアタイム記録） */
void recStageTime(int player) {
	if(stage[player] > 26) return;
	//TOMOYOステージタイムctime[
	stage_time[stage[player] + player * 200] = ctime[player];
	if(st_record_interval_tgm==10){
		st_other[stage[player] +player*30] = st_bdowncnt[player];//接着回数
		st_bdowncnt[player] = 0;//リセット
	}
}
/* ステージクリアできないタイム記録） */
void recFaultTime(int player) {
	if(stage[player] > 26) return;
	//TOMOYOステージタイムctime[
	stage_time[stage[player] + player * 200] = 5400;
	if(st_record_interval_tgm==10){
		st_other[stage[player] +player*30] = 99;//接着回
	}
	st_bdowncnt[player] = 0;
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.07 - ゲームオーバー演出
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statGameOver(int player) {
	int		i, j, block, c;

	// 下入れ制限強制解除#1.60c7f9
	down_flag[player] = 0;
	up_flag[player] = 0;

	// ROTリレーで死んだとき
	if((gameMode[player] == 9) && (relaymode[player]) && (!ending[player])){
		stat[player] = 15;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 1;	//窒息演出
		return;
	}

	// PRACTICEで死んだとき
	if(gameMode[player] == 5) {
		flag = -1;
		return;
	}
	// 対戦で死んだとき
	if(gameMode[player] == 4) {
		flag = -3;
		winr = 0;
		winc = 0;
		winu = - 24;
		wink = 0;
		if(IsPlayWave(65)){
			StopWave(65);
			PlayWave(50 +bgmlv);
		}
		if(stat[1 - player] != 7){
			vs_points[1 - player]++;
			PlaySE(41);
			objectCreate(1 - player, 14, 230 + 20*((hnext[1 - player] >= 4) && (1 - player == 0)) + 44 * (1 - player) - 96,26 + (12 * vs_points[1 - player]),0,0,13,0);
		}
		if(vs_points[1 - player] >= winpoint){
			flag = - 2;
			if(!demo) vs_win[1 - player]++;	// #1.60c7n1
		}
		return;
	}

	//警告音が鳴っていたら止める
	StopSE(40);
	StopSE(32);

	if((gameMode[player]==6)&&(tomoyo_opt[player]==0)){
		recFaultTime(player);//ステージNOTランキング用タイム
	}

	statc[player * 10 + 1]++;

	hiddeny[player] = 22;

	if( statc[player * 10] > 22 ) {
		if(statc[player * 10 + 1] == 141) {
			//PlayWave(8);

			if( (stat[1 - player] == 0) || (stat[1 - player] == 10) ) {
				if(wavebgm) {
					StopAllBGM();
				} else {
					if(IsPlayMIDI()) StopMIDI();
				}
				statc[player * 10 + 2] = 0;
			} else {
				//statc[player * 10 + 2] = 1;		//2人同時プレイフラグON
			}
		}

		tlv[player] = lv[player] - slv[player];

		if(statc[player * 10 + 1] >= 60) {
			if((gameMode[player] == 8)||(cpu_flag[player])){
				// MISSION用
				stat[player] = 14;//ゲームオーバー
			} else if(gameMode[player] == 6) {
				// TOMOYO用 #1.60c7l6
				stat[player] = 20;//コンテニュー
			} else {
				stat[player] = 11;// ネームエントリー
			}
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 3] = 0;
			statc[player * 10 + 4] = 0;
			statc[player * 10 + 5] = 0;
			// CPU解除 #1.60c7o3
			cpu_flag[player] = 0;
			if(ending[player] == 2){
			if(((gameMode[player] == 0) && (novice_mode[player])) || ((gameMode[player] == 3) && (devil_minus[player]))){
				stat[player] = 22;
				statc[player * 10] = 3740 - edrec[player];
				statc[player * 10 + 1] = 13;
			}
			}
			if(examination[player]){
				stat[player] = 30;	//段位試験時
				statc[player * 10 + 1] = 1;		//結果発表へ
			}
		}

		return;
	}

	if(statc[player * 10] < 0){
		statc[player * 10]++;
		return;
	}

	if(statc[player * 10 + 1] > 4) {
		block = statc[player * 10];
		if(block == 0) {
			if((deadtype) || (heboGB[player])) PlaySE(7);	// 下から消去は音はなし #1.60c7h3
		}

		statc[player * 10 + 1] = 0;
		for(j = 0; j < 10; j++) {
			// ブロックの色変更で、LV1000ブロックはほかのブロックとは別のものにする。 #1.60c7d
			// 死亡アニメを選べるようにした#1.60c7h3
			if((deadtype) || (heboGB[player])) {
				if(block <= fldsizeh[player]){
					c = fld[j + block * 10 + player * 220];
					if(c != 0) {
						if((c >= 9) || (c < 0))
							fld[j + block * 10 + player * 220] = 10;
						else
							fld[j + block * 10 + player * 220] = 1;
					}
				}
			} else {
				// スーッと消えていく演出 #1.60c7k6
				// +=のような代入演算子はつかえねぇ‥‥cond ? true : falseも駄目‥‥
				if(21 - block >= 0) {
					c = fldt[j + (210 - (block * 10)) + player * 220];
					if((c <= -1) || (c > 8)) c = 8;
					fldt[j + (210 - (block * 10)) + player * 220] = c - 4;
				}
				if(block > 0) {
					c = fldt[j + (210 - (block - 1) * 10) + player * 220];
					fldt[j + (210 - (block - 1) * 10) + player * 220] = c - 4;
				}
			}
		}
		statc[player * 10]++;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.08 - ブロック消去
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// Big対応 waitの扱いを忠実にした#1.60c7 バグ取り #1.60c7b
void statEraseBlock(int player) {
	int		i, j, k, l, x, y, ty, lines, wait, fldtmp, hardblock,hardblock2,tmp_s[2];
	hardblock = 0;
	hardblock2 = 0;

	if(repversw >= 17) padRepeat(player);	// #1.60c7s6

	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	// 正方形判定
	if(squaremode[player]) {
		tmp_s[player] = makeSquare(player);
		if( (((mission_type[c_mission] == 40) && (tmp_s[player] >= 1)) || ((mission_type[c_mission] == 41) && (tmp_s[player] >= 2))&& (!ending[player])) ){
			missionNormUp(2);
		}
	}

	ty = 0;
	if((gameMode[player] != 10) || (rots[player] == 8)) wait = 1;
	else wait = (wait2[player] > 12) * 9 + 1;

	hiddenProc(player);
//	UpLineBlockJudge(player);

	// ↓を離している場合、下入れ制限解除#1.60c7n6
	if((!getPressState(player, 1-(1* ((isUDreverse[player]) && (repversw>=44)) ))) && (repversw >= 10)) down_flag[player] = 0;
	if(!getPressState(player, 0+(1*((isUDreverse[player]) && (repversw>=44)) ))) up_flag[player] = 0;

	if(statc[player * 10] < wait) {
		y = fldsizeh[player];
		lines = 0;

		for(i = 0; i <= fldsizeh[player]; i++) {
			if(erase[i + player * 22] != 0) {
				if(erase[i + player * 22] != 2) lines++;	// 2の時は対戦のDEFENCEで消えるライン
				y--;
				if(wait != 1) {//ORIGINALモード
					x = statc[player * 10];

					if(breakeffect) {
						objectCreate(player, 1, (x + 15 + 24 * player - 12 * maxPlay) * 8, (i + 3) * 8, (x - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + lines * 250, fld[x + i * 10 + player * 220], 100);
					}
					fld[x + i * 10 + player * 220] = 0;
					fldt[x + i * 10 + player * 220] = 0;	// #1.60c7j5

				} else {
					if(erase[i + player * 22] == 2)
						objectCreate(player, 13, (15 + 24 * player - 12 * maxPlay) * 8, (i + 3) * 8, 0, 0, 0, 0);
					for(x = 0; x < fldsizew[player]; x++) { // #1.60c7b
						fldtmp = fld[x + i * fldsizew[player] + player * 220];

						if(breakeffect) {
							if((erase[i + player * 22] == 1) && (fldi[x + i * 10 + player * 220] != fldihardno)){
								// super_breakeffectが1の場合は全てのブロックに破壊エフェクトを適用する #1.60c7m9
								// プラチナブロックが消える場合は無条件
								if( (fld[x + i * fldsizew[player] + player * 220] >= 11) || (super_breakeffect == 1) ||
									( ((breaktype == 0)||((breaktype == 3)&&(gameMode[player] == 0))) && (super_breakeffect == 2) ) ||
									((heboGB[player] != 0) && (super_breakeffect == 2)) ) {
									objectCreate(player, 1, (x + 15 + 24 * player - 12 * maxPlay) * 8, (i + 3) * 8, (x - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + lines * 250, fld[x + i * 10 + player * 220], 100);
								} else if(lines & 1) {
									if((x & 1) == 1) {
										objectCreate(player, 1 + (wait1[player] < 6) * 2, (x + 15 + 24 * player - 12 * maxPlay) * 8 + 4, (i + 3) * 8, (x - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + lines * 250, fld[x + i * 10 + player * 220], 100);
									}
								} else {
									if((x & 1) == 0) {
										objectCreate(player, 1 + (wait1[player] < 6) * 2, (x + 15 + 24 * player - 12 * maxPlay) * 8 + 4, (i + 3) * 8, (x - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + lines * 250, fld[x + i * 10 + player * 220], 100);
									}
								}
								if(fldi[x + i * fldsizew[player] + player * 220])//アイテムが消えるときの白いエフェクト
									if(fldi[x + i * 10 + player * 220] < fldihardno)
										objectCreate(player, 12, (x + 15 + 24 * player - 12 * maxPlay) * 8, (i + 3) * 8, 0, 0, 0, 100);
							}
						}

						// 消えるブロックがプラチナブロックなら時間延長 #1.60c7l5
						if( (fld[x + i * fldsizew[player] + player * 220] >= 11) && (gameMode[player] == 6)) {
							if(stage[player] < 100) ltime[player] = ltime[player] + 60;
							else sc[player] = sc[player] + 100;
							platinaerase_cnt[player] = platinaerase_cnt[player] + 10;
						}

						// アイテムが消える #1.60c7n1
						if(fldi[x + i * fldsizew[player] + player * 220] ) {
							if(fldi[x + i * 10 + player * 220] == fldihardno){// ハードブロックを消す
								hardblock = 1;
								hardblock2 = 1;
							//	erase[i + player * 22] = 0;	//落下キャンセル
							}else if(fldi[x + i * 10 + player * 220] == fldisno){	// シルバースクウェア
								li[player] = li[player] +  5/4;
							}else if(fldi[x + i * 10 + player * 220] == fldigsno){	// ゴールデンスクウェア
								li[player] = li[player] +  5/2;
							}else if(fldi[x + i * 10 + player * 220] < fldihardno){	// ハードブロック以下がアイテム
								item_waiting[player] = fldi[x + i * 10 + player * 220];

							}
						}

						if(!hardblock){
							fld[x + i * 10 + player * 220] = 0;
							fldt[x + i * 10 + player * 220] = 0;	// #1.60c7j5
							grayoutLackedBlock(player, flds[x + i * 10 + player * 220]);
							flds[x + i * 10 + player * 220] = 0;
						}
						hardblock = 0;
						fldi[x + i * fldsizew[player] + player * 220] = 0;	// #1.60c7n1
					}
					if(hardblock2) erase[i + player * 22] = 0;	//落下キャンセル
					hardblock2 = 0;
					if(ty == 0) ty = i * (IsBig[player] + 1);

					tomoyoCheckStageClear(player);	//ステージクリア判定 #1.60c7l8
					if((item[player] != 0) && (item[player] != fldihardno) && (repversw >= 55))
						item_waiting[player] = item[player];
				}
			}
		}/* for(i = 0; i <= fldsizeh[player]; i++) */

//		statc[player * 10]++;

		// 消えるラインが無かった場合
		if(y == fldsizeh[player]) {
			stat[player] = 6;
			statc[player * 10] = wait1[player];
			statc[player * 10 + 1] = 15;
			cmbpts[player] = 0;
			combo[player] = 0;
			hebocombo[player] = 0;
			combo2[player] = 0;
			hanabi_combo[player] = 0;
			scrate[player] = 100;//oriモード用のスコアレート
			pinchCheck(player);
			pinchCheck2(player);
		// 1ライン以上消えた場合
		} else if(statc[player * 10] == 0) {
			// BIGモードの場合、消去ライン数を2で割る #1.60c7m6
			if( (IsBig[player]) && (BigMove[player] == 1) && (lines >= 2) ) lines = lines / 2;
			if((tspin_flag[player] == 2) && (lines > 3) && (repversw >= 59)) lines = 3;

			if((lines < 4) && (tspin_flag[player] < 2)){	//Back to Back終了
				b_to_b_flag[player]=0;
			}

			// 花火
			if( ((gameMode[player] == 0)||((gameMode[player] == 10)&&(ori_opt[player]==0)))
			 && (!no_hanabi) && (!hanabi_flag)&&(!novice_mode[player]) ) {
				if((lines >= 2)||((tspin_flag[player]!= 0)&&(lines != 0))) {//
					if((tspin_flag[player] != 0)&&(lines != 0)){		//Tスピン
						hanabi_combo[player] = ((hanabi_combo[player] + lines) * 5)/2 ;
					}

					i = lines + hanabi_combo[player] + combo2[player];
					hanabi_combo[player] = i;		// 前に打ち上げた数を覚えておく

					if(ending[player] == 2) i = i * 2;		// ロール中は2倍
					else if(tc[player] >= 150)   i = i + (i/2);		// LV150以降は1.5倍

					hanabi_total[player] = hanabi_total[player] + i;
					hanabi_waiting[player] = hanabi_waiting[player] + i;


				} else {
					// T-spinなしは一列消しはコンボ中でも一発
					hanabi_total[player]++;
					hanabi_waiting[player]++;
				}

				hanabi_frame[player] = 30;
				hanabi_flag = 1;
			}

			if( lines >= 5 )
				PlaySE(10 + 4);		// 5ライン以上の場合は4ラインの音を出す
			else
				PlaySE(10 + lines);

			if(lines >= 4) {
				if((!ending[player]) &&(medaltype != 2)&&(gameMode[player] != 0) &&(gameMode[player] != 4) && (gameMode[player] <= 5) ) {
					skill[player]++;
					if(gameMode[player] == 3) {
						if( ( (skill[player] == 7) && (medal_sk[player] == 0) )
						 || ( (skill[player] == 13) && (medal_sk[player] == 1) )
						 || ( (skill[player] == 18) && (medal_sk[player] == 2) )
						 || ( (skill[player] == 25) && (medal_sk[player] == 3) ))
						 {
							PlaySE(39);
							medal_sk[player]++;
							objectCreate2(player, 11,208 + 81 * player - 96 * maxPlay,135,2,0,0,medal_sk[player] - 1);
						}
					} else {
						skillg[player]++;
						if( ( (skill[player] == 15) && (medal_sk[player] == 0) )
						 || ( (skill[player] == 27) && (medal_sk[player] == 1) )
						 || ( (skill[player] == 37) && (medal_sk[player] == 2) )
						 || ( (skill[player] == 44) && (medal_sk[player] == 3) ))
						 {
							PlaySE(39);
							medal_sk[player]++;
							objectCreate2(player, 11,208 + 81 * player - 96 * maxPlay, 135,2,0,0,medal_sk[player] - 1);
						}
					}
				}
			}

			if(comboM[player] <= combo2[player]){//コンボ数大きい方を保持
				comboM[player] = combo2[player];

				// メダル獲得音
				if( (lines >= 2) && (!ending[player]) &&(medaltype != 2)&&(gameMode[player] != 0) &&(gameMode[player] != 4) && (gameMode[player] <= 5) ) {
					// #1.60c7m9
					if( ( (comboM[player] == 3) && (medal_co[player] == 0) )
					 || ( (comboM[player] == 4) && (medal_co[player] == 1) )
					 || ( (comboM[player] == 6) && (medal_co[player] == 2) ) )
					{
						PlaySE(39);
						medal_co[player]++;
						objectCreate2(player, 11,233 + 31 * player - 96 * maxPlay, 148,5,0,0,medal_co[player] - 1);
					}
				}
			}
			if(hebocombo[player]) PlaySE(18);
			combo[player]++;
			cmbpts[player] = cmbpts[player] + (lines * 2 - 2);
			if(lines >= 2) {
				// 1ライン消しを含まないコンボ #1.60c7m8
				combo2[player]++;
			}
			if((combo2[player] >= 3) && (ending[player] != 2) && (lines >= 2) && (gameMode[player] <= 4))
				PlaySE(18);
			if((tc[player] < 500)&&(combo2[player] >= 4)&&(lines >= 2)){
				skillg[player]++;	//ポイントを上げるための倍率
			}else if((tc[player] >= 500)&&(combo2[player] >= 3)&&(lines >= 2)){
				skillg[player]++;
			}
			if((showcombo != 0) || (gameMode[player] == 10)){
				objectComboClearPl(player);
				if(lines >= 4)
					objectCreate((b_to_b_flag[player] != 0) + (tspin_flag[player] == 2), 2, 156 + player * 120 - 96 * maxPlay,
					 100 + 32 * player, - 900 * (player * 2 - 1), - 600, 4, combo2[player] * isComboMode(player));
				else
					objectCreate((b_to_b_flag[player] != 0) + (tspin_flag[player] == 2), 2, 156 + player * 120 - 96 * maxPlay,
					 100 + 32 * player, - 900 * (player * 2 - 1), - 600, lines, combo2[player] * isComboMode(player));
			}
			// T-SPIN獲得 #1.60c7n6
			// エンディングでも獲得できる #1.60c7n8
			if(tspin_flag[player]) {
				PlaySE(34);
				last_tspin[player]=lines * (tspin_flag[player] == 2);
				tspin_c[player] = 60 * tspin_flag[player];
			}
			if(b_to_b_flag[player])
					b_to_b_c[player] = 120;
			// スコア計算を「ライン消しをした瞬間」に変更
			if((gameMode[player] != 10) && ((!ending[player])||(gameMode[player]==5))) {
				calcScore(player,lines);
			}
			//ロール中列消しカウント
			if((ending[player] == 2)&&(enable_grade[player] >= 2)){
				endlines[player] = endlines[player] + lines;
			}
			//Back to Back の開始判定(およびSE選択)
			//終了判定は1ライン以上消えた瞬間（5179行目あたり）に
			if(lines >= 4){
				if(b_to_b_flag[player] == 0){
					if(b2bcheck)
						b_to_b_flag[player]=1; //Back to Back判定開始
					PlaySE(17);
				}
				else{ //B to B Heboris
					b_to_b_flag[player]=1;
					PlaySE(18);
				}
			}
			if((tspin_flag[player] == 2) && (b2bcheck == 1) && (lines >= 1)){
				PlaySE(48);
				if(b_to_b_flag[player] == 0){
					PlaySE(17);
					b_to_b_flag[player]=lines+1; //Back to Back判定開始
				}
				else{ //B to B T-Spin erase
					PlaySE(18);
					b_to_b_flag[player]=lines+1;
				}
			}
			if(ismiss[player]) PlaySE(45);
			//ターゲットでステージクリア後に地形が残ってしまうバグを修正(とりあえず応急処置) #C7T9.6EX
			if((gameMode[player]==8) && (mission_type[c_mission] == 6) && (stat[0] == 25)) return;
		}
	}

	if(statc[player * 10] >= wait2[player] + (repversw < 27)) {
		y = fldsizeh[player];
		lines = 0;

		for(i = 0; i <= fldsizeh[player]; i++) {
			if(erase[y + player * 22]) {
				for(k = y; k > 0; k--) {
					for(l = 0; l < fldsizew[player]; l++) {
						fld[l + k * fldsizew[player] + player * 220] = fld[l + (k - 1) * fldsizew[player] + player * 220];
						// #1.60c7j5
						fldt[l + k * fldsizew[player] + player * 220] = fldt[l + (k - 1) * fldsizew[player] + player * 220];
						// #1.60c7n1
						fldi[l + k * fldsizew[player] + player * 220] = fldi[l + (k - 1) * fldsizew[player] + player * 220];
						flds[l + k * fldsizew[player] + player * 220] = flds[l + (k - 1) * fldsizew[player] + player * 220];

					}
					erase[k + player * 22] = erase[(k - 1) + player * 22];
				}
				for(l = 0; l < fldsizew[player]; l++) {
					fld[l + player * 220] = 0;
					fldt[l + player * 220] = 0;		// #1.60c7j5
					fldi[l + player * 220] = 0;		// #1.60c7n1
					flds[l + player * 220] = 0;

				}
				erase[player * 22] = 0;
				lines++;
			} else {
				y--;
			}
		}
		if(gameMode[player] == 10) {
			calcScore(player,lines);
		}

		PlaySE(2);

		erasecnt[player]++;		//消去回数カウント #C7T9.6EX
		if(ismiss[player]){
			ofs_x[player] = 0;
			ofs_x2[player] = 0;
		}
		// アイテム発動
		eraseItem(player, item_waiting[player]);

		//エンディング突入音 #1.60c7j9
		if( (ending[player] == 1) || (ending[player] == 4) ||(ending[player] >= 6)) PlaySE(28);
		stat[player] = 6;
		statc[player * 10] = wait1[player];
		statc[player * 10 + 1] = 15;
		if((gameMode[player] == 9) && (relaymode[player]) || (gameMode[player] == 5)) StopSE(28);

		pinchCheck(player);
		pinchCheck2(player);
	} else {
		//D.R.SのAREスキップ
		if( ((rots[player] == 8) && (harddrop[player])) &&
		((getPushState(player, 0)) || (getPushState(player, 1)) || (getPushState(player, 2)) ||
		 (getPushState(player, 3)) || (getPushState(player, 4)) || (getPushState(player, 5)) ||
		 (getPushState(player, 6)) || (getPushState(player, 7))) ){
		 	statc[player * 10] = wait2[player];
		 	are_skipflag[player] = 1;
	 	}
		if(ismiss[player]){
			y = Rand(6);
			ofs_x[player] = y - (6 / 2);
			ofs_x2[player] = ofs_x[player];
		}
		statc[player * 10]++;
	}
	if((repversw >= 55) && (ending[player] == 4) && (gameMode[player] != 8)){
		PlaySE(18);
		stat[player] = 13;
		for(i = 0; i < 10; i++) statc[i + player * 10] = 0;
		return;
	}
}
// 破片ブロック判定
void grayoutLackedBlock(int player, int no) {
	int i, j;

	if((gameMode[player] < 8) && (gameMode[player] != 5)) return;
	if(no <= 0) return;

	for(j = 0; j < 22; j++) {
		for(i = 0; i < fldsizew[player]; i++) {
			if((fld[i + j * 10 + player * 220] >= 1)&&  (flds[i + j * 10 + player * 220] == no)) {
				flds[i + j * 10 + player * 220] = 0;	// 破片にする
				if(squaremode[player]) fld[i + j * 10 + player * 220] = 1;	// 灰色ブロックにする
			}
		}
	}
}
// スクウェアでの3コーナーT-SPIN時用　全て破片化
void grayoutAllBlock(int player) {
	int i, j;

	if((gameMode[player] < 8) && (gameMode[player] != 5)) return;

	for(j = 0; j < 22; j++) {
		for(i = 0; i < fldsizew[player]; i++) {
			if(fld[i + j * 10 + player * 220] >= 1) {
				flds[i + j * 10 + player * 220] = 0;	// 破片にする
				if((fldi[i + j * 10 + player * 220] == fldigsno) || (fldi[i + j * 10 + player * 220] == fldisno)){
					fldi[i + j * 10 + player * 220] = 0;	//正方形解除
				}
				if(squaremode[player]) fld[i + j * 10 + player * 220] = 1;	// 灰色ブロックにする
			}
		}
	}
}
/// 正方形ができるかチェック
// player：プレイヤー番号 x:始点のX座標 y:始点のY座標 gold:1の場合は一種類のブロックのみで正方形ができる
int checkSquare(int player, int x, int y, int gold) {
	int i, j, color;

	// 始点の色を取得
	color = getFieldBlock(player, x, y,0);

	// 空白だったら失敗
	if(color <= 0) return 0;

	// おじゃまブロックや初期配置だったり、破片だったり、既に正方形だったら失敗
	if(getFieldBlockS(player, x, y) <= 0) return 0;

	// 色が2種類以上なら失敗（gold != 0のときのみ）
	if(gold) {
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++) {
				if(getFieldBlock(player, x + i, y + j,0) != color) return 0;
			}
		}
	}

	// 破片があれば失敗
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(getFieldBlockS(player, x + i, y + j) <= 0) return 0;
		}
	}

	// 始点より上にブロックが繋がってたら失敗
	for(i = 0; i < 4; i++) {
		if(getFieldBlockS(player, x + i, y    ) == getFieldBlockS(player, x + i, y - 1)) return 0;
	}

	// 正方形の底より下にブロックが繋がってたら失敗
	for(i = 0; i < 4; i++) {
		if(getFieldBlockS(player, x + i, y + 3) == getFieldBlockS(player, x + i, y + 4)) return 0;
	}

	// 始点より左にブロックが繋がってたら失敗
	for(j = 0; j < 4; j++) {
		if(getFieldBlockS(player, x    , y + j) == getFieldBlockS(player, x - 1, y + j)) return 0;
	}

	// 正方形の端より右にブロックが繋がってたら失敗
	for(j = 0; j < 4; j++) {
		if(getFieldBlockS(player, x + 3, y + j) == getFieldBlockS(player, x + 4, y + j)) return 0;
	}

	// 成功
	return 1;
}

// 正方形を作る
int makeSquare(int player) {
	int i, j, k, l, result;
	result = 0;

	for(i = 0; i < 22 - 3; i++) {
		for(j = 0; j < 10 - 3; j++) {
			if(checkSquare(player, j, i, 1)) {
				// 金色
				if(result < 2) result = 2;
				PlaySE(18);
				squarecnt[player]=squarecnt[player]+2;
				// マークを付ける
				for(k = 0; k < 4; k++) {
					for(l = 0; l < 4; l++) {
						fldi[(k + j) + (l + i) * 10 + player * 220] = fldigsno;//定数
						flds[(k + j) + (l + i) * 10 + player * 220] = -1;
					}
				}
			} else if(checkSquare(player, j, i, 0)) {
				// 銀色
				if(result < 1) result = 1;
				PlaySE(17);
				squarecnt[player]++;
				// マークを付ける
				for(k = 0; k < 4; k++) {
					for(l = 0; l < 4; l++) {
						fldi[(k + j) + (l + i) * 10 + player * 220] = fldisno;
						flds[(k + j) + (l + i) * 10 + player * 220] = -1;
					}
				}
			}
		} /* for(j = 0; j < 6; j++) */
	} /* for(i = 0; i < 18; i++) */

	return result;
}
// 正方形用
int getFieldBlockS(int player, int bx1, int by1) {
	if((bx1 < 0) || (bx1 > 9) || (by1 < 0) || (by1 > 21)) return 0;

	return flds[bx1 + by1 * 10 + player * 220];
}

int isComboMode(int player){
	if((gameMode[player] <= 5) || (gameMode[player] == 10)) return 1;
	else if(gameMode[player] == 8) return (mission_type[c_mission] == 27);
	else return 0;
}
/* スコア計算 #1.60c7j9 */
void calcScore(int player, int lines) {
	int		i, j, k, l, x, y, bo[2], bai, all[2], tcbuf = 0,btmup,top;

		/* 全消し処理 */
		all[player] = 1;
		// 全消しチェック 最下段だけでなく全部見る#1.60c7g7
		for(y = fldsizeh[player]; y > 0; y--) {
			// 消去した段はチェックしない（動作速度向上？）#1.60c7o8
			if(erase[y + player * 22] == 2){	//VSのDEFENCEで消えるラインはノーカウント
				all[player] = 0;
				break;
			}else if(!erase[y + player * 22]) {
				for(x = 0; x < fldsizew[player]; x++) {
					if(fld[x + fldsizew[player] *y + player * 220]) {
						all[player] = 0;
						break;
					}
				}
			}
		}
		// 全消し演出
		if(all[player]) {
			objectCreate2(player, 4, player, 30, 0, 0, 0, 0);
			if((gameMode[player] == 8) && (mission_type[c_mission] == 26)){
				missionNormUp(1);
				return;
			}

			// heboris.iniで全消しエフェクトを無効にできるようにした#1.60c7f6
			if(bravo) {
				for(i = -3; i <= 3; i++)
					for(j = 0; j < 5; j++)
						objectCreate(player, 5, 62 + player * 196, 64 + j * 30 + (i % 2) * 15, 180 * i, - 2000 + j * 200, Rand(7) + 1, 1);
			}
		}
		if((squaremode[player])&&(tspin_flag[player] == 2)){//本当は部分フリーフォルらしいけどわからないから全部
				grayoutAllBlock(player);	// 全て灰色化
				for(i = 0;i < 22;i++) erase[player * 22 + i] = 0;
				stat[player] = 32;
				statc[player * 10 + 0] = 0;
				statc[player * 10 + 1] = 4;		// ステータス
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;
				statc[player * 10 + 4] = 0;
		}
	if(gameMode[player] == 10 ) {
		/* ライン加算 */
			li[player] = li[player] + lines;

		if ( lines == 1 ){
			bo[player] = 1 * scrate[player];
			scrate[player] = scrate[player] + 5;
		}

		if ( lines == 2 ){
			bo[player] = 4 * scrate[player];
			scrate[player] = scrate[player] + 10;
		}

		if ( lines == 3 ){
			bo[player] = 9 * scrate[player];
			scrate[player] = scrate[player] + 30;
		}

		if ( lines >= 4 ){
			bo[player] =  20 * scrate[player];
			scrate[player] = scrate[player] + 50;
		}

					/* スコア計算 */
			if(((rots[player] == 3) || (rots[player] == 4) || (rots[player] == 5) || (rots[player] == 6)) && (repversw >= 20))
				qdrop[player] = qdrop[player] / 2;		//高速落下速度の遅いルールは、ドロップボーナス半減
			if(((rots[player] == 7) || (rots[player] == 8)) && (repversw >= 45))
				qdrop[player] = qdrop[player] + (qdrop[player] / 2);		//高速落下速度が速いルールは、ドロップボーナス5割増

			bai = 1 + ( sp[player] >= 20 ) + ( sp[player] >= 60 ) + ( sp[player] >= 240 ) + ( sp[player] >= 600 );
			if(all[player])	bai = bai * 10;
			sc[player] = sc[player] + bo[player]*bai + sdrop[player] + qdrop[player];
		checkFadeout(player);
	}else if(( gameMode[player] == 7 )||(gameMode[player] == 9)) {
		/* ライン加算 */
			li[player] = li[player] + lines;
		if((gameMode[player]==7)&&(anothermode[player]==3)){
					/* スコア計算 */
			if(((rots[player] == 3) || (rots[player] == 4) || (rots[player] == 5) || (rots[player] == 6)) && (repversw >= 20))
				qdrop[player] = qdrop[player] / 2;		//高速落下速度の遅いルールは、ドロップボーナス半減
			if(((rots[player] == 7) || (rots[player] == 8)) && (repversw >= 45))
				qdrop[player] = qdrop[player] + (qdrop[player] / 2);		//高速落下速度が速いルールは、ドロップボーナス5割増
			sc[player]=sc[player] + sdrop[player] + qdrop[player];
			if(lines==1)bo[player] = 100;
			else if(lines==2)bo[player] = 400;
			else if(lines==3)bo[player] = 900;
			else if(lines>=4)bo[player] = 2000;

			if(lv[player]<8){
			bai = (lv[player]+2)/2;
			}else{
			bai = 5;
			}
			if(all[player])	bai = bai * 10;
			sc[player] = sc[player] + bo[player]*bai;
		}
		if(gameMode[player]==9){
			bai=1;//初期倍率
			if(tspin_flag[player]){
				bo[player] = 4 * ( last_tspin[player] + 1 );
			}else if(!tspin_flag[player]){
				if(lines==1)bo[player] = 1;
				else if(lines==2)bo[player] = 3;
				else if(lines==3)bo[player] = 5;
				else if(lines>=4)bo[player] = 8;
			}
			//Back to Back Heborisで1.5倍
			if((b_to_b_flag[player] != 0) && (lines >= 4)){
				bai = 3*bai ;
			}
			if(lv[player]==0)lv[player]=sp[player]/100;
			c_norm[player] = c_norm[player] + bo[player] * bai/(1+(b_to_b_flag[player]!=0)) ;//goalにレベルアップ用ノルマを入れる
			sc[player] = sc[player] + bo[player] * bai/(1+(b_to_b_flag[player]!=0)) *100*lv[player];//得点
		}
		checkFadeout(player);
	} else if( (gameMode[player] == 8) && (mission_type[c_mission] != 26)&& (mission_type[c_mission] != 40)&& (mission_type[c_mission] != 41)) {
		missionNormUp(lines);
		return;
	} else if( gameMode[player] != 6 ) {
		if( gameMode[player] != 4 ) {
			/* スコア計算 */
			if(((rots[player] == 3) || (rots[player] == 4) || (rots[player] == 5) || (rots[player] == 6)) && (repversw >= 20))
				qdrop[player] = qdrop[player] / 2;		//高速落下速度の遅いルールは、ドロップボーナス半減
			if(((rots[player] == 7) || (rots[player] == 8)) && (repversw >= 45))
				qdrop[player] = qdrop[player] + (qdrop[player] / 2);		//高速落下速度が速いルールは、ドロップボーナス5割増
			bo[player] = (tc[player] + lines) / 4 + sdrop[player] + qdrop[player];
			bai = (lines * 2 - 1 + cmbpts[player] - (lines * 2 - 2)) * lines;
			// 全消ししたら4倍
			if(all[player]) {
				bai = bai * 4;
				if(  (!ending[player]) &&(medaltype != 2)&&(gameMode[player] != 0) &&(gameMode[player] != 4) && (gameMode[player] <= 5) ) {
					allclear[player]++;		// 全消し回数増加 #1.60c7k4
					if( ( (allclear[player] == 1) && (medal_ac[player] == 0) )
			 		|| ( (allclear[player] == 2) && (medal_ac[player] == 1) )
					|| ( (allclear[player] == 3) && (medal_ac[player] == 2) )
					|| ( (allclear[player] == 4) && (medal_ac[player] == 3) ))
			 		{
			 			PlaySE(39);
						medal_ac[player]++;
						objectCreate2(player, 11,208 + 81 * player - 96 * maxPlay, 122,0,0,0,medal_ac[player] - 1);
					}
				}
			}
			//Back to Back Heborisで1.5倍
			if((b_to_b_flag[player] != 0) && (lines >= 4)){
				bai = bai + (bai/2);
			}
			//T-spin Eraseで4倍(DS式のみ)
			if((tspin_flag[player] == 2) && (lines >= 1) && (tspin_type>=2)){
					bai = bai * 4;	//通常時4倍
				if(b_to_b_flag[player] != 0)
					bai = bai + (bai/2);	//Back to Backでさらに1.5倍
			}
			//noviceだと6倍
			if((novice_mode[player])&&(gameMode[player] == 0)){
				bai = bai * 6;
			}

			/* ライン加算 */
			if((!ismiss[player]) || (repversw < 62)){
				sc[player] = sc[player] + bo[player]*bai;
				li[player] = li[player] + lines;

				/* 段位上昇 */
				if(enable_grade[player] == 1){
					GradeUp(player);
				}else if((enable_grade[player] == 2)||(enable_grade[player] == 3)){
					GradeUp2(player,lines);
				}else if(enable_grade[player] == 4){//
					GradeUp3(player);
				}
			}
		}else{
			li[player] = li[player] + lines;
		}
	}
	/* heboレベル用 */
	lc[player] = lc[player] + lines * 25;

	if((gameMode[player] == 6) && (blind_flag[player]))
		isblind[player] = !isblind[player];

	if(((gameMode[player] == 5) || (playback)) && (lines != 0)){	//PRACTICEでは消去ラインをカウント
		if((lines <= 3) && (tspin_flag[player] != 2))
			p_erase_c[lines-1] = p_erase_c[lines-1] +1;	//通常の1、2、3ライン消し
		else if((lines >= 4) && ((b_to_b_flag[player] == 0) || (!b2bcheck)))
			p_erase_c[3] = p_erase_c[3] +1;				//ヘボリス
		else if((lines >= 4) && (b_to_b_flag[player]))
			p_erase_c[4] = p_erase_c[4] +1;				//B to B ヘボリス
		else if((tspin_flag[player] == 2) &&  ((b_to_b_flag[player] == 0) || (!b2bcheck)) && (tspin_type>=2))
			p_erase_c[4+lines] = p_erase_c[4+lines] +1;	//T-スピンerase(DS式のみ)
		else if((tspin_flag[player] == 2) && (b_to_b_flag[player] != 0) && (tspin_type>=2))
			p_erase_c[7+lines] = p_erase_c[7+lines] +1;	//B to B T-スピンerase(DS式のみ)
	}

	if((enable_grade[player] == 2)&&(lines >= 2)){
		if((tc[player] < 500)&&(combo2[player] >= 4)){
			skillg[player]++;	//ポイントを上げるための倍率
		}else if((tc[player] >= 500)&&(combo2[player] >= 3)){
			skillg[player]++;
		}
	}
	if((tspin_flag[player])&&(enable_grade[player] == 2)){//Tspinも段位うｐの一部
		if(lines != 0){
			skillg[player]++;
		}else if(lines == 3){
			skillg[player] = skillg[player]+3;
		}else {
			skillg[player] = skillg[player]+2;
		}
	}
	if((tspin_flag[player])&&(enable_grade[player] == 4)){//Tspinも段位うｐの一部
		 if(lines > 0)
			skillg[player]++;
	}

	// 対戦モードでのせり上がりカウント（MOVE ERASED以外） #1.60c7n1
	if((gameMode[player] == 4) && (upLineT[1 - player] != 3)) {
		if(vs_style[player] == 1){//ATTACK
			if(lines == 2){
				upLines[1 - player] = upLines[1 - player] + 3;
			}else if(lines == 3){
				upLines[1 - player] = upLines[1 - player] + 4;
			}else if(lines >= 4){
				upLines[1 - player] = upLines[1 - player] + lines + 2;
				if(b_to_b_flag[player] != 0){
					upLines[1 - player] = upLines[1 - player] + 1 +(vs_style[player] == 1);
				}
			}
		}else {
			if(lines == 2){//その他
				upLines[1 - player] = upLines[1 - player] + 1;
			}else if(lines == 3){
				upLines[1 - player] = upLines[1 - player] + 2;
			}else if(lines >= 4){
				upLines[1 - player] = upLines[1 - player] + lines;
				if(b_to_b_flag[player] != 0){
					upLines[1 - player] = upLines[1 - player] + 1 +(vs_style[player] == 1);
				}
			}
		}
		//T-Spinでライン消しすると攻撃力2倍になるようせり上がり数を追加（DS式のみ）#1.60c7T1.1
		if((tspin_flag[player] == 2) && (tspin_type>=2)){
			if(lines == 1)
				upLines[1 - player] = upLines[1 - player] + 2 + (vs_style[player] == 1);
			else if(lines == 2)
				upLines[1 - player] = upLines[1 - player] + 3;
			else if(lines >= 3)
				upLines[1 - player] = upLines[1 - player] + 4;
			if(b_to_b_flag[player] != 0){
				upLines[1 - player] = upLines[1 - player] + 1 + (vs_style[player] == 1);
			}
		}
		// 全消しした場合は攻撃力2倍
		if(all[player])
			upLines[1 - player] = upLines[1 - player] * 2;
		// ダブルの時も二倍
		if(isdouble[1 - player])
			upLines[1 - player] = upLines[1 - player] * 2;
		if(disrise)//せりあがり禁止
			upLines[1 - player] = 0;
		if((!noitem) && (disrise)){		//アイテムオンリーではゲージ上昇
			item_g[player] = item_g[player] + lines * (1 + ((tspin_flag[player] == 2) && (repversw >= 62)));
			if(item_g[player] > item_inter[player])
				item_g[player] = item_inter[player];
		}
	// MOVE ERASEDせり上がり
	} else if(gameMode[player] == 4){
		// 2ライン以上、またはDS式T-Spin消しで実行
		top = upLines_waiting[1 - player];
		btmup = 0;
		if( (!disrise) && ((lines >= 2) || ((tspin_flag[player] == 2) && (tspin_type >= 2) && (lines >= 1))) ){
			// 追加せり上がりの計算
			if((tspin_flag[player] == 2) && (tspin_type >= 2) || (all[player])){	//DS式T-Spin
				for(i = 0; i <= upLines_waiting[1 - player]; i++) {
					for(j = 0; j < fldsizew[player]; j++) {	//左右反転して1セット追加
						if(vs_style[player] <= 1) k = (fldsizew[player] - 1) - j;
						else k = j;
						fldubuf[k + (i + lines + (vs_style[player] == 1)) * fldsizew[player] + (1 - player) * 220] = fldubuf[j + i * fldsizew[player] + (1 - player) * 220];
					}
				}
				upLines_waiting[1 - player] = upLines_waiting[1 - player] + lines + (vs_style[player] == 1);
			}
			if(b_to_b_flag[player] != 0){	//Back to Back （DEFENCE以外）
				for(j = 0; j < fldsizew[player]; j++) {	//最上段を左右反転した1ラインを追加(T-Spinでは反転させない)
					if((tspin_flag[player] == 2) && (tspin_type>=2) || (all[player]) || (vs_style[player] > 1)) k = j;
					else k = (fldsizew[player] - 1) - j;
					fldubuf[k + (upLines_waiting[1 - player]) * fldsizew[player] + (1 - player) * 220] = fldubuf[j + 0 * fldsizew[player] + (1 - player) * 220];
				}
				upLines_waiting[1 - player] = upLines_waiting[1 - player] + 1;
				if(vs_style[player] == 1){	//ATTACK
					for(j = 0; j < fldsizew[player]; j++) {	//もう1ラインを追加(
						if((tspin_flag[player] == 2) && (tspin_type>=2) || (all[player])||(isdouble[1 - player])) k = (fldsizew[player] - 1) - j;
						else k = j;
						fldubuf[k + (upLines_waiting[1 - player]) * fldsizew[player] + (1 - player) * 220] = fldubuf[j + 0 * fldsizew[player] + (1 - player) * 220];
					}
					upLines_waiting[1 - player] = upLines_waiting[1 - player] + 1;
				}
			}
			if(isdouble[1 - player]){	//DOUBLEによるせり上げ2倍
				for(i = 0; i <= upLines_waiting[1 - player]; i++) {
					for(j = 0; j < fldsizew[player]; j++) {	//左右反転させず1セット追加
						fldubuf[j + (i + upLines_waiting[1 - player]) * fldsizew[player] + (1 - player) * 220] = fldubuf[j + i * fldsizew[player] + (1 - player) * 220];
					}
				}
				upLines_waiting[1 - player] = upLines_waiting[1 - player] * 2;
			}
			//バッファからせり上がりフィールドへ送る
			if(upLines[1 - player] + 1 >= 22) btmup = 1;
			for(i = 0; i < upLines_waiting[1 - player]; i++) {
				if(btmup) {
					for(k = 1; k < 22; k++) {
						for(l = 0; l < 10; l++) {
							fldu[l + ( k - 1 ) * 10 + (1 - player) * 220] = fldu[l + k * 10 + (1 - player) * 220];
						}
					}
				}
				for(j = 0; j < fldsizew[player]; j++) {
					fldu[j + upLines[1 - player] * fldsizew[player] + (1 - player) * 220] = fldubuf[j + i * fldsizew[player] + (1 - player) * 220];
				}
				if(upLines[1 - player] < 21) upLines[1 - player]++;
				else btmup = 1;
			}
		}
		upLines_waiting[1 - player] = 0;
		if((!noitem) && (disrise)){		//アイテムオンリーではゲージ上昇
			item_g[player] = item_g[player] + lines * (1 + ((tspin_flag[player] == 2) && (repversw >= 62)));
			if(item_g[player] > item_inter[player])
				item_g[player] = item_inter[player];
		}
	}

	// HIDDENで隠された段を見えるようにする
	hiddenti[player] = hiddenti[player] + lines;

	while(hiddenti[player] > 0) {
		hiddenti[player]--;
		hiddeny[player]++;
		if(hiddeny[player] > 22) {
			hiddeny[player] = 22;
			hiddentm[player] = 0;
		}
	}

	// 4ライン以上消えた場合
	if(lines >= 4) {
		hebocombo[player] = 1;	// 4ライン消しコンボ開始
	}

	if((gameMode[player] == 5) && (p_ending!=0))
		return;		//PRACTICEエンディング中はここまで

	/* TGMレベルを上げる */
	if( gameMode[player] <= 6 ) {
		tcbuf = tc[player];

		// tgmカウンタ ライン分上昇(3, 4列消しでボーナス加算) #1.60c
		// レベルアップボーナスがTIの場合のみプラスする #1.60c7g3

		if(repversw >= 50){
			if(lvupbonus==2){//自動調整
				if(novice_mode[player]||( ((gameMode[player]==1)||(gameMode[player]==2)) &&(enable_grade[player]!=4))
				||(devil_minus[player])){tc[player] = tc[player] + lines;
				}else {tc[player] = tc[player] + lines + ((lines > 2) * (lines - 2));}
			}else if(lvupbonus==1)
				tc[player] = tc[player] + lines;
			else
				tc[player] = tc[player] + lines+ ((lines > 2) * (lines - 2));;	// tgmカウンタ ライン分上昇
		}else{
			if(lvupbonus)
				tc[player] = tc[player] + lines ;
			else
				tc[player] = tc[player] + lines+ ((lines > 2) * (lines - 2));	// tgmカウンタ ライン分上昇

		}
		if((ismiss[player]) && (repversw >= 62)) tc[player] = tcbuf;

		if((!fpbas_mode[player])&&(gameMode[player]<=6)) lv[player] = tc[player];

		// レベルストップ音を鳴らす #1.60c7j8
		if((gameMode[player] == 4) && (repversw >= 61) && (wintype==0) && (vs_goal != 0)){
			if(tc[player] == vs_goal - 1) PlaySE(9);
		}else if((!fpbas_mode[player])&&(gameMode[player]<=6)) {
			if((repversw > 4) && (gameMode[player] >= 3))
				i = 99;
			else
				i = 99 -  (tc[player] > 900);

			if(tc[player] % 100 == i) PlaySE(9);
		}
	}

	// エンディング突入チェック
	checkEnding(player, tcbuf);

	// レベルアップ（エンディング中なら何も行われない）
	LevelUp(player);
}

/* エンディング突入判定 */
void checkEnding(int player, int tcbuf) {

	// MASTERと20GではTGMレベルが999に達したらフラグON
	if((gameMode[player] >= 1) && (gameMode[player] <= 2) && (tc[player] >= 999)) {
		// 段位認定#1.60c7g5
		// gm条件が成立するとシャドウロールになる		#1.60c7i2
		if(enable_grade[player] == 1) {
			if((sc[player] > gscore[17]) && (time[player] <= 810 * 60) && (gmflag1[player]) && (gmflag2[player])) {
				objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				PlaySE(43);
				gmflag_r[player] = 1;	// GMになる権利が与えられる #1.60c7i2
			}
		} else if(enable_grade[player] == 2) {
		//M以上で8分45秒以内
			if((grade[player] >= 27) && (time[player] <= 525 * 60) && (gmflag1[player]) && (gmflag2[player])) {
				objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				PlaySE(43);
				gmflag_r[player] = 1;	// GMになる権利が与えられる #1.60c7i2
			}
		}  else if(enable_grade[player] == 3) {
		//M以上で8分45秒以内
			if((grade2[player] >= 29) && (time[player] <= 525 * 60) && (gmflag1[player]) && (gmflag2[player])) {
				objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				PlaySE(43);
				gmflag_r[player] = 1;	// GMになる権利が与えられる #1.60c7i2
			}
		} else if(enable_grade[player] == 4) {
		//m5以上で6分30秒以内
		//skillcoolが6個以上、regretが一回も出ていない
			if((grade[player] >= 22) && (time[player] <= 390 * 60) && (gup3rank[player] == 2) && (gmflag1[player]) && (!gmflag2[player])) {
				objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				PlaySE(43);
				gmflag_r[player] = 1;	// GMになる権利が与えられる #1.60c7i2
			}
		} else {
			gmflag_r[player] = 1;
			ending[player] = 1;
		}

		tc[player] = 999;
		lv[player] = tc[player];
		timeOn[player] = 0;
		if((enable_grade[player] == 1) || ((hebo_plus[player]) && (repversw >= 58))){
			ending[player] = 6;
		}else {
			ending[player] = 1;
		}

		end_f[player] = 1;

		// スタッフロール用wait値セット#1.60c7j5
		sp[player] = speed_master_roll;

		if(((repversw > 24)&&(enable_grade[player] == 1))||((hebo_plus[player]==1)&&(repversw>=34)&&(repversw<42)&&(gameMode[player] ==1))){
			//特別なモードの時はかえない
			wait1[player] = wait1_beginner_roll;
			wait2[player] = wait2_beginner_roll;
			wait3[player] = wait3_beginner_roll;
			waitt[player] = waitt_beginner_roll;
		}else if(repversw <= 38) {//38まで
			wait1[player] = wait1_master_roll38;
			wait2[player] = wait2_master_roll38;
			wait3[player] = wait3_master_roll38;
			waitt[player] = waitt_master_roll38;
		}else if(enable_grade[player] != 4) {//段位4設定以外
			wait1[player] = wait1_master_roll;
			wait2[player] = wait2_master_roll;
			wait3[player] = wait3_master_roll;
			waitt[player] = waitt_master_roll;
		}else if(enable_grade[player] == 4) {//段位4設定
			wait1[player] = wait1_master_rollG4;
			wait2[player] = wait2_master_rollG4;
			wait3[player] = wait3_master_rollG4;
			waitt[player] = waitt_master_rollG4;
		}
		// 900～999のセクションタイム記録#1.60c7k7
		tr[player] = 100;
		recSectionTime(player);
	// BEGINNERなら200で終了させる#1.60c7n2
	} else if((gameMode[player] == 0) && (tc[player] >= 200)&&(!novice_mode[player])) {
		//PlayWave(28);
		tc[player] = 200;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 1;
		end_f[player] = 1;

		//ロール中の花火間隔決定　打ち上げは上のほう
		if(hanabi_total[player] >=400) hanabi_int[player] = 12;//秒間5発
		else if((hanabi_total[player] >= 200) && (hanabi_total[player] <= 399)) hanabi_int[player] = 15;//秒間4発
		else hanabi_int[player] = 20;//秒間3発

		// スタッフロール用wait値セット#1.60c7j5
		sp[player] = speed_beginner_roll;
		wait1[player] = wait1_beginner_roll;
		wait2[player] = wait2_beginner_roll;
		wait3[player] = wait3_beginner_roll;
		waitt[player] = waitt_beginner_roll;

		//PlayWave(19);
		if(backno <= 1) {
			bgfadesw = 1;
		}
	// NOVICEなら300で終了させる
	} else if((gameMode[player] == 0) && (tc[player] >= 300) && (novice_mode[player])) {
		//PlayWave(28);

		// タイムボーナス
		if((novice_mode[player])&&(time[player] < 18000)) {
			sc[player] = sc[player] + (1253 * (300 - (time[player] / 60)));
		}
		tc[player] = 300;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 6;
		end_f[player] = 1;

		// スタッフロール用wait値セット#1.60c7j5
		sp[player] = speed_beginner_roll;
		wait1[player] = wait1_beginner_roll;
		wait2[player] = wait2_beginner_roll;
		wait3[player] = wait3_beginner_roll;
		waitt[player] = waitt_beginner_roll;

		//PlayWave(19);
		if(backno <= 1) {
			bgfadesw = 1;
		}
	// DEVIL-なら1000で終了させる#1.60c7u0.9
	} else if((((enable_grade[player] == 1) && (repversw < 42)) || (devil_minus[player]))&&(gameMode[player] == 3)&&(repversw >= 31) && (tc[player] >= 1000)) {

		//PlayWave(28);
		tc[player] = 1000;
		grade[player] = 10; // Grade設定 #1.60c7j7
		gflash[player]=120;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 6;
		end_f[player] = 0;//ラインなし
		// スタッフロール用wait値セット#1.60c7j5
		if(repversw >= 54){
			wait1[player] = wait1_devil_tbl[8];
			wait2[player] = wait2_devil_tbl[8];
			wait3[player] = wait3_devil_tbl[8];
			waitt[player] = waitt_devil_tbl[8];
		}
	// DEVILなら1300で終了させる#1.60c7f3
	} else if((gameMode[player] == 3) && (tc[player] >= 1300)) {
		if(( ((!isWRule(player)) && (time[player] <= 19200)) || ((isWRule(player)) && (time[player] <= 21000)) )&&(GetRealFPS()>40)) {
			grade[player] = 16;
			objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
			PlayWave(43);
		}//ネ申条件を満たしていたらその時点でS13に

		if(death_plus[player]){
			grade[player] = 16;
			gflash[player]=120;
		}else{
			grade[player] = 13; // Grade設定 #1.60c7j7
			gflash[player]=120;
		}
		tc[player] = 1300;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 1;
		end_f[player] = 1;

		// スタッフロール用wait値セット#1.60c7j5
		if(repversw >= 57){
			sp[player] = speed_devil_roll;
			wait1[player] = wait1_devil_roll;
			wait2[player] = wait2_devil_roll;
			wait3[player] = wait3_devil_roll;
			waitt[player] = waitt_devil_roll;
		}else if(repversw >= 39){
			sp[player] = speed_devil_roll;
			wait1[player] = wait1_devil_roll56;
			wait2[player] = wait2_devil_roll56;
			wait3[player] = wait3_devil_roll56;
			waitt[player] = waitt_devil_roll56;
		}else if(repversw>18){
			sp[player] = speed_devil_roll;
			wait1[player] = wait1_devil_roll38;
			wait2[player] = wait2_devil_roll38;
			wait3[player] = wait3_devil_roll38;
			waitt[player] = waitt_devil_roll38;
		}else{
			sp[player] = speed_devil_roll;
			wait1[player] = wait1_devil_roll17;
			wait2[player] = wait2_devil_roll17;
			wait3[player] = wait3_devil_roll17;
			waitt[player] = waitt_devil_roll17;
		}

		// このバージョンから強制BIG化#1.60c7h5
		if(repversw > 4) {
			IsBig[player] = 1;
		}
		// DEVIL-の500で終了させる#1.60c7v0
	} else if(  (devil_minus[player])&&(gameMode[player] == 3)&&(repversw >= 52) && (tc[player] >= 500) && (tcbuf < 500) &&
			( ((time[player] > timelimitm[player]) && (timelimitm[player] > 0) && ( !isWRule(player) )) ||	// TGM,Ti
			  ((time[player] > timelimitmw[player]) && (timelimitmw[player] > 0) && (isWRule(player) )) )  ) {

		//PlayWave(28);
		tc[player] = 500;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 6;
		end_f[player] = 0;//ラインなし
		if(repversw >= 54){
			wait1[player] = wait1_devil_m_tbl[8];
			wait2[player] = wait2_devil_m_tbl[8];
			wait3[player] = wait3_devil_m_tbl[8];
			waitt[player] = waitt_devil_m_tbl[8];
		}
	// 500で足きり#1.60c7g7
	} else if(((gameMode[player] == 1) || (gameMode[player] == 2)) && //マスター20G
				(tc[player] >= 500) && (tcbuf < 500) &&
				(time[player] > timelimit[player]) && (timelimit[player] > 0)) {
		//PlayWave(28);
		tc[player] = 500;
		bgfadesw = 1;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 4;
		onRecord[player] = 2;					// エンディングロール終了と同時にリプレイ記録終了
	// devilは二つ足切り#1.60c7r2b
	} else if(  ((gameMode[player] == 3) && (!devil_minus[player]) &&//devil
				(tc[player] >= 500) && (tcbuf < 500)) &&
				 ( ( (time[player] > timelimit[player]) && (timelimit[player] > 0) && (!isWRule(player)) )||	// TGM,Ti
				 ( (time[player] > timelimitw[player]) && (timelimitw[player] > 0) && (isWRule(player)) ) )	// ワールド系
				) {
		//PlayWave(28);
		tc[player] = 500;
		bgfadesw = 1;
		grade[player]++;	//S5になる
		gflash[player]=120;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 4;
		onRecord[player] = 2;					// エンディングロール終了と同時にリプレイ記録終了
	// LV1000で足切り #1.60c7q8
	} else if((gameMode[player] == 3) && (!devil_minus[player]) &&
				(tc[player] >= 1000) && (tcbuf < 1000) &&
				(time[player] > timelimit2[player]) && (timelimit2[player] > 0)) {
		tc[player] = 1000;
		bgfadesw = 1;
		grade[player]++;	//S10になる
		gflash[player]=120;
		lv[player] = tc[player];
		timeOn[player] = 0;
		ending[player] = 4;
		onRecord[player] = 2;					// エンディングロール終了と同時にリプレイ記録終了
	// ACE
	} else if((gameMode[player] == 7) && (li[player] >= 150) && (anothermode[player] <= 1)) {
		li[player] = 150;
		timeOn[player] = 0;
		ending[player] = 1;
		end_f[player] = 2;
		onRecord[player] = 2;
	// ANOTHER2 (Lv20)
	} else if((gameMode[player] == 7) && (li[player] >= 200) && (anothermode[player] == 2)) {
		li[player] = 200;
		timeOn[player] = 0;
		ending[player] = 1;
		end_f[player] = 2;
		onRecord[player] = 2;
	//practis
	} else if(gameMode[player] == 5){
		if(p_goaltype==1){//レベル
			if(tc[player] >= p_goaltypenumlist[p_goaltypenum]*10){
				tc[player] = p_goaltypenumlist[p_goaltypenum]*10;
				lv[player] = tc[player];
				ending[player] = 7;		// 超短縮
			}
		} else if(p_goaltype==2){//ライン
			if(li[player] >= p_goaltypenumlist[p_goaltypenum]){
				li[player] = p_goaltypenumlist[p_goaltypenum];
				ending[player] = 7;		// 超短縮
			}
		} else if(p_goaltype==3){//ブロック
			if(bdowncnt[player] >= p_goaltypenumlist[p_goaltypenum]){
				bdowncnt[player] = p_goaltypenumlist[p_goaltypenum];
				ending[player] = 7;		// 超短縮
			}
		} else if(p_goaltype==4){//タイム
			if(ltime[player] <= 0){
				lv[player] = tc[player];
				ending[player] = 7;		// 超短縮
			}
		}
		if(ending[player] == 7){
			timeOn[player] = 0;
			onRecord[player] = 2;
		}
	// 40Line
	}else if((gameMode[player] == 9) &&(std_opt[player]==0)&& (li[player] >= 40)) {
		li[player] = 40;
		timeOn[player] = 0;
		ending[player] = 7;
		end_f[player] = 2;
		onRecord[player] = 2;
	// ULTRA
	} else if((gameMode[player] == 9) &&(std_opt[player]==1)&& (ltime[player] <= 0)) {
		timeOn[player] = 0;
		ending[player] = 7;
		end_f[player] = 2;
		onRecord[player] = 2;
	// SQUARE
	}else if((gameMode[player] == 9) &&(std_opt[player]==2)&& (li[player] >=150)&& (lv[player] >=15)) {
		timeOn[player] = 0;
		ending[player] = 1;
		end_f[player] = 2;
		onRecord[player] = 2;
	}else if((gameMode[player] == 9) &&(std_opt[player]==3)&& (li[player] >=75)&& (lv[player] >=15)) {
		timeOn[player] = 0;
		ending[player] = 1;
		end_f[player] = 2;
		onRecord[player] = 2;
	}

}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.10 - 対戦モード相手決定待ち
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statVersusWait(int player) {
	// 何もしないようにした(PRACTICEの2P用) #1.60cd

	// 対戦モード復活につき変更#1.60c7g1
	if(gameMode[0] == 4) {
		if(count % 60 > 30) {
			ExBltRect(81, 110+192 * player -96 * maxPlay , 91 , 100, 0, 100, 50);
		}

		//回転法則のランダムセレクト
		if((versus_rot[player] == 9) && (stat[1 - player] != 38)){
			rots[player] = Rand(9);
			setNextBlockColors(player, 1);
		}

		if((stat[0] == 10) && (player == 1)) {
			vs_match++;		// #1.60c7n1
			first_rot[0] = rots[0];
			first_rot[1] = rots[1];
			stat[0] = 3;
			stat[1] = 3;
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.11 - ネームエントリー
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statNameEntry(int player) {
	int		move, len, j,k;
	int		i, rank, add;		// #1.60c7i5
	int		st_update,cat[2],ex[2];
	int		temp[2],temp2[2],color[2];

	add = 0;

	// 判定基準を決める
	if(gameMode[player] == 0) {
		if(!novice_mode[player]){
		temp[player] = hanabi_total[player];//花火総数		// BEGINNER
		}else{
		temp[player] = sc[player];//スコア		// BEGINNER
		}
		temp2[player] = tc[player];
	}else if( (gameMode[player] >= 1) && (gameMode[player] <= 2) ) {
		temp[player] = grade[player];//段位		// MASTER & 20G
		temp2[player] = tc[player];//レベル
	} else if(gameMode[player] == 3) {//devil
		temp[player] = tc[player];	// レベル
		temp2[player] = grade[player];//段位
	}else if(gameMode[player] == 6) {//tomoyo
		if(tomoyo_opt[player]<2){
			temp[player] = stage[player];	// 到達ステージ
			temp2[player] = clearp[player];//クリア率
		}else{
			temp[player] = stage[player];	// 到達ステージ
			temp2[player] = sc[player];//スコア
		}
	} else if(gameMode[player] == 7) {//ace
		temp[player] = li[player];	//ライン
		temp2[player] = lv[player]-1;	//レベル
	} else if(gameMode[player] == 9) {//STD
		if(std_opt[player]<2){
			temp[player] = li[player];	//ライン
			temp2[player] = bdowncnt[player];	//ブロック
		}else if(std_opt[player]==2){
			temp[player] = squarecnt[player];	//スクウェア数
			temp2[player] =sc[player];	//スコア
		}else if(std_opt[player]==3){
			temp[player] = sc[player];	//ライン
			temp2[player] = lv[player];	//スコア
		}
	}else if(gameMode[player]==10){
		temp[player] = sc[player];					//
		temp2[player] = ori_opt[player];
	}else{
		temp[player] = 0;					// ランキングなし		temp[player] = tc[player];
		temp2[player] = 0;
	}
	// 順位を取得する
	if(ranking_type==0){
		rank = RankingCheck(gameMode[player],0,sc[player],time[player],lv[player],(ending[player]==3));
	}else if(ranking_type==1){
	//モードと設定からカテゴリ設定
		if(gameMode[player]==0){
		cat[player] = 0;
		}else if(gameMode[player]==1){
			if(enable_grade[player]==1){
			cat[player] = 1;
			}else if(enable_grade[player]==2){
			cat[player] = 2;
			}else if(enable_grade[player]==3){
			cat[player] = 3;
			}else if(enable_grade[player]==4){
			cat[player] = 4;
			}
		}else if(gameMode[player]==2){
			if(enable_grade[player]<=3){
			cat[player] = 5;
			}else if(enable_grade[player]==4){
			cat[player] = 6;
			}
		}else if(gameMode[player]==3){
			if(devil_minus[player]==0){
			cat[player] = 7;
			}else if(devil_minus[player]==1){
			cat[player] = 8;
			}
		}else if(gameMode[player]==6){
			if(tomoyo_opt[player]==0){
			cat[player] = 9;
			}else if(tomoyo_opt[player]==1){
			cat[player] = 10;
			}
		}else if(gameMode[player]==7){
			if(anothermode[player]==0){
			cat[player] = 11;
			}else if(anothermode[player]==1){
			cat[player] = 12;
			}else if(anothermode[player]==2){
			cat[player] = 13;
			}
		}
		rank = RankingCheck2(cat[player], rots[player], temp[player], time[player], end_f[player]);
	}else{
			//モードと設定からカテゴリ設定
		if(gameMode[player]==0){
			if(novice_mode[player]){
				cat[player] = 0;
				ex[player] = 1;
			}else{
				cat[player] = 0;
				ex[player] = 0;
			}
		}else if(gameMode[player]==1){
			if(enable_grade[player]==1){
				cat[player] = 1;
				ex[player] = 0;
			}else if(enable_grade[player]==2){
				cat[player] = 1;
				ex[player] = 1;
			}else if(enable_grade[player]==3){
				cat[player] = 2;
				ex[player] = 0;
			}else if(enable_grade[player]==4){
				cat[player] = 2;
				ex[player] = 1;
			}
		}else if(gameMode[player]==2){
			if(enable_grade[player]<=3){
				cat[player] = 3;
				ex[player] = 0;
			}else if(enable_grade[player]==4){
				cat[player] = 3;
				ex[player] = 1;
			}
		}else if(gameMode[player]==3){
			if(devil_minus[player]==0){
				cat[player] = 4;
				ex[player] = 0;
			}else if(devil_minus[player]==1){
				cat[player] = 4;
				ex[player] = 1;
			}
		}else if(gameMode[player]==6){
			if(tomoyo_opt[player]==0){
				cat[player] = 5;
				ex[player] = 0;
			}else if(tomoyo_opt[player]==1){
				cat[player] = 5;
				ex[player] = 1;
			}else if(tomoyo_opt[player]==2){
				cat[player] = 6;
				ex[player] = 0;
			}else if(tomoyo_opt[player]==3){
				cat[player] = 6;
				ex[player] = 1;
			}
		}else if(gameMode[player]==7){
			if(anothermode[player]==0){
				cat[player] = 7;
				ex[player] = 0;
			}else if(anothermode[player]==1){
				cat[player] = 7;
				ex[player] = 1;
			}else if(anothermode[player]==2){
				cat[player] = 8;
				ex[player] = 0;
			}else if(anothermode[player]==3){
				cat[player] = 8;
				ex[player] = 1;
			}
		}else if(gameMode[player]==10){
			cat[player] = 9;
			ex[player] = 0;
		}else if(gameMode[player]==9){
			if(std_opt[player]==0){
				cat[player] = 10;
				ex[player] = 0;
			}else if(std_opt[player]==1){
				cat[player] = 10;
				ex[player] = 1;
			}else if(std_opt[player]==2){
				cat[player] = 11;
				ex[player] = 0;
			}else if(std_opt[player]==3){
				cat[player] = 11;
				ex[player] = 1;
			}
		}
		rank = RankingCheck3(cat[player],ex[player], rots[player], temp[player], time[player], end_f[player]);


		if((gameMode[player]==10)&&(rank==-1)){//oriでは4位以下も調べる
			cat[player] = 9;
			ex[player] = 1;
			rank = RankingCheck3(cat[player],ex[player], rots[player], temp[player], time[player], end_f[player]);
		}
			// TOMOYOモードで1面以外から始めた場合は記録に残さない
		if(gameMode[player]==6){
			if(((tomoyo_opt[player]==0) && (start_stage[player] != 0)) || ((tomoyo_opt[player]==1) && (start_stage[player] != 27))||(tomoyo_opt[player] == 3)){
				stat[player] = 21;
				rank = -1;
			}
		}
		// アイテムと+は残さない
		// TOMOYOのターゲットとかも残さない
		if(IsBigStart[player] || item_mode[player] || hebo_plus[player] || relaymode[player]){
			stat[player] = 21;
			rank = -1;
		}

	}


	// セクションタイム更新チェック
	// TOMOYOのEH以降は残さない（筈）
	if( playback || demo || IsBigStart[player] || item_mode[player] || hebo_plus[player] || ((gameMode[player] == 6) && (tomoyo_opt[player] != 0))||(gameMode[player] == 0)||(gameMode[player] >= 7)) {
		st_update = 0;
	} else {
		st_update = ST_RankingCheckAll(player, gameMode[player], enable_grade[player]);
	}


	// ランク外なら即リザルトへ
	if( (rank == -1)&& (!st_update) ) {
		if(gameMode[player] == 6)
			stat[player] = 21;
		else if(show_result)
			stat[player] = 14;
		else
			stat[player] = 21;

		statc[player * 10] = 0;
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		return;
	}

	// 音楽を流す #1.60c7l2
	// 2人同時で重ならないように修正 #1.60c7m1
	if( ((stat[1 - player] == 0) || (stat[1 - player] == 10)) && (!IsPlayWave(63)) && (wavebgm > 0) )
		PlayWave(63);
/*
	// リプレイセーブ#1.60c7i5
	if((textguide) && (!playback) && (!demo)){
		if(!player) printFont(13+24 * player - 12 * maxPlay, 26, "0-9 KEY:REPLAY", 0);
		else printFont(13+24 * player - 12 * maxPlay, 26, "Q-P KEY:REPLAY", 0);
	}
	ReplaySaveCheck(player, 3);

	add = 3 * ((enable_grade[player] == 4) && (sgrade[player] >= min_sgrade) && ((gameMode[player] == 1) || (gameMode[player] == 2)));

	if(statc[player * 10 + 3]) {
		if( sgrade[player] >= min_sgrade ) {	//裏段位表示時に重なるので
			sprintf(string[0], "SAVED : %02d", statc[player * 10 + 3]);
			printFont(15 + 24 * player - 12 * maxPlay, 24, string[0], 5);
		} else {
			printFont(15 + 24 * player - 12 * maxPlay, 20, "YOUR PLAY", 7);
			printFont(16 + 24 * player - 12 * maxPlay, 21, "WAS SAVED", 7);
			sprintf(string[0], "FILE NO.%02d", statc[player * 10 + 3]);
			printFont(15 + 24 * player - 12 * maxPlay, 23, string[0], 5);
		}
	}
*/
	// GRADE #1.60c7m9
		if((enable_grade[player] == 4) && ((gameMode[player] == 1) || (gameMode[player] == 2))) {
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 137 - (add * 3), 228, 119, 26, 7);

				if(grade[player] == 32) {
					// Gm
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 144 - (add * 3), 270 ,((count % 4 / 2)*216)+120,90,48);
				}else if( grade[player] >= 28 ) {
					// MK-MM
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 144 - (add * 3), 270 ,((count % 4 / 2)*216)+((grade[player]-28)*24),90,24);
				}  else if( grade[player] == 27 ) {
					//Master
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 144 - (add * 3), 270 ,((count % 4 / 2)*216),90,24);
				} else if( grade[player] >= 18 ) {
					// m1～m9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 144 - (add * 3), 180 ,((count % 4 / 2)*216)+((grade[player]-18)*24),90,24);
				} else if( grade[player] >= 9 ) {
					// S1～S9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 144 - (add * 3), 90 ,((count % 4 / 2)*216)+((grade[player]-9)*24),90,24);
				} else {
					// 9～1
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 144 - (add * 3), 0 ,((count % 4 / 2)*216)+(grade[player]*24),90,24);
				}
		}
		// S-GRADE #1.60c7o1
		if( sgrade[player] >= min_sgrade ) {
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 160, 220, 91, 31, 7);
			ExBltRect(3, 152 + 192 * player - 96 * maxPlay, 160, 228, 119, 26, 7);
			if( sgrade[player] == 18 ) {
				// GM
				ExBltRect(26,115 + 192 * player -96 * maxPlay , 168, 270 ,((count % 4 / 2)*216)+168,90,24);
			} else if( sgrade[player] >= 9 ) {
				// S1～S9
				ExBltRect(26,115 + 192 * player -96 * maxPlay , 168, 90 ,((count % 4 / 2)*216)+((sgrade[player]-9)*24),90,24);
			} else {
				// 9～1
				ExBltRect(26,115 + 192 * player -96 * maxPlay , 168, 0 ,((count % 4 / 2)*216)+(sgrade[player]*24),90,24);
			}
		}
	if(statc[player * 10 + 1] == 0)
		StrCpy(string[player + 2], "");

	len = StrLen(string[player + 2]);

	// 何位に入ったか表示#1.60c7i5
	if(rank != -1) {
		sprintf(string[0], "RANK %d", rank + 1+(3*((cat[player] == 9)&&(ex[player]==1))));
		printFont(17 + 24 * player - 12 * maxPlay, 8-add, string[0], 5);
	}
	// セクションタイム更新
	if(st_update) {
		printFont(15 + 24 * player - 12 * maxPlay, 9-add, "NEW ST REC", 3);
	}

	printFont(15 + 24 * player - 12 * maxPlay, 10-add, "ENTER NAME", 4);

	if((stat[1] == 10) && (player == 0)) {
		stat[0] = 3;
		stat[1] = 3;
	}

	statc[player * 10 + 1]++;

	if(statc[player * 10 + 1] < 45 * 60){
		padRepeat(player);

		move = getPressState(player, 3) - getPressState(player, 2);
		if(move)
			if((mpc[player] == 1) || (mpc[player] >= 15)) {
				if(mpc[player] >= 15) mpc[player] = 12;
				statc[player * 10 + 2] = statc[player * 10 + 2] + move;
				PlaySE(5);
			}

		if(statc[player * 10 + 2] > 54)
			statc[player * 10 + 2] = (len == 3) * 53;
		if(statc[player * 10 + 2] < (len == 3) * 53)
			statc[player * 10 + 2] = 54;

		MidStr(RankString, statc[player * 10 + 2] + 1, 1, string[4]);

	//	statc[player * 10 + 1]++;

		printFont(18 + 24 * player+len - 12 * maxPlay, 14-add, string[4], 2 * (count % 20 > 10));
		printFont(18 + 24 * player - 12 * maxPlay, 14-add, string[player + 2], 0);
		sprintf(string[0], "%2d", 45 - (statc[player * 10 + 1] / 60));
		printFont(16 + 24 * player - 12 * maxPlay, 16-add, "TIME", 6);
		printFont(22 + 24 * player - 12 * maxPlay, 16-add, string[0], 0);

		for(k = 0;k <= 9;k++){
			if(k > statc[player * 10 + 1] / 2) break;
			j = statc[player * 10 + 2]-3-len + k;
			MidStr(RankString, j + 1 + (55 * (j < 0)) - (55 * (j > 54)), 1, string[0]);
			if(j==statc[player * 10 + 2]){
				printFont(15 + k + 24 * player - 12 * maxPlay, 12-add, string[0], 2 * (count % 20 > 10));
				printFont(15 + k + 24 * player - 12 * maxPlay, 13-add, "n", count % 9);
			} else printFont(15 + k + 24 * player - 12 * maxPlay, 12-add, string[0], 0);
		}
		if(getPushState(player, 4)) {
			PlaySE(10);
			if(statc[player * 10 + 2] == 53) {
				if(len) {
					MidStr(string[player + 2], 1, len - 1, string[player + 2]);
				}
			} else if(statc[player * 10 + 2] == 54) {
				statc[player * 10 + 1] = 45 * 60;
			} else
				StrCat(string[player + 2], string[4]);
		}
	} else{
		printFont(18 + 24 * player - 12 * maxPlay, 14-add, string[player + 2], (count % 4 / 2) * digitc[rots[player]]);
	}

	if(statc[player * 10 + 1] >= 45 * 60) {
		if(statc[player * 10 + 1] == 45 * 60) {
			if(!len) StrCpy(string[player + 2], "NOP");
			PlaySE(18);
		}
		if(statc[player * 10 + 1] == (46 * 60) + 30){
			sprintf(string[player + 2], "%s   ", string[player + 2]);
			LeftStr(string[player + 2], 3, string[player + 2]);

			if(ranking_type==0){
				RankingRegist(gameMode[player], 0, sc[player], li[player], lv[player], time[player], (ending[player]==3), string[player + 2]);
				RankingSave();

			}else if(ranking_type==1){
				RankingRegist2(cat[player], rots[player], temp[player], time[player],  end_f[player], temp2[player], string[player + 2], rank, medal_ac[player], medal_st[player], medal_sk[player], medal_co[player],medal_re[player]);
				RankingSave2();
			}else {
				RankingRegist3(cat[player], ex[player],rots[player], temp[player], time[player],  end_f[player], temp2[player], string[player + 2], rank, medal_ac[player], medal_st[player], medal_sk[player], medal_co[player],medal_re[player]);
				RankingSave3();
			}

			SaveConfig();

			// エンディング到達チェック
			if(ending[player] == 3) {
				i = 2;
			}
			else if ( ((gameMode[player] == 0) &&(novice_mode[player] ==0) &&(tc[player] == 200))  ||
			((gameMode[player] == 0) &&(novice_mode[player] ==1) &&(tc[player] == 300))  ||
					  ((gameMode[player] == 3) && (tc[player] == 1300)) ||
					  ((gameMode[player] >= 1) && (gameMode[player] <= 2) && (tc[player] == 999))
					)
			{
				i = 1;
			}
			else {
				i = 0;
			}
			// セクションタイム更新
			if(st_update) {
				ST_RankingUpdate(player, gameMode[player], i, enable_grade[player]);

			}
			ST_RankingSave();
			PlayerdataSave();

			if(gameMode[player] == 6){
				if(!playback){
					stat[player] = 38;
				}else{
					stat[player] = 21;	// ゲームオーバー表示 #1.60c7p1
				}
			}else{
				stat[player] = 14;	// 結果画面へ #1.60c7p1
			}
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 3] = 0;
		}
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.13 - エンディング・ロール
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statEnding(int player) {
	int	i, j, k;
//sprintf(string[0],"END %2d %2d",ending[player],gameMode[player]);
//printFont(0, 0, string[0], (count % 4 / 2) * digitc[rots[i]]);
	if ( (ending[player] == 1) || (ending[player] == 4) ){
		// 音楽停止 #1.60c7j9
if (bgmteisiflg == 0) {
bgmteisiflg = 1;
		if((gameMode[player] != 8) || ((gameMode[player] == 8) && (mission_end[c_mission ] >= 4)))
		StopAllBGM();
		pinch[player] = 0;
		// ACEでの残り時間が少ない時の効果音も停止
		StopSE(32);
}
//sprintf(string[0],"-STOP- %2d %2d",ending[player],gameMode[player]);
//printFont(0, 1, string[0], (count % 4 / 2) * digitc[rots[i]]);
		if((onRecord[player]) && (repversw >= 59)) padRepeat(player);

		//上に移動
		if((gameMode[player] == 6) && (stage[player] == 199)){
			printFont(15 + 24 * player - 12 * maxPlay, 11, "COMPLETE!", (count % 4 / 2) * digitc[rots[player]]);
			printFont(14 + 24 * player - 12 * maxPlay, 14, "+100000 PTS", (count % 4 / 2) * digitc[rots[player]]);
		}

		// ブロックを消す
		// BIG対応 #1.60c7j5
		if(statc[player * 10] % 6 == 0) {
			j = (fldsizeh[player] + 1) - (statc[player * 10] / 6);
			if((j >= 0) && (j <= fldsizeh[player])) {
			for(i = 0; i < fldsizew[player]; i++) {
				// ライン消しエフェクトで消える #1.60c7n5
				if( fld[i+ j * fldsizew[player] + player * 220] != 0) {
					objectCreate(player, 1, (i + 15 + 24 * player - 12 * maxPlay) * 8,(j + 3) * 8, (i - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + 1 * 250, fld[i+ j * fldsizew[player] + player * 220], 100);
				}

				fld[i+ j * fldsizew[player] + player * 220] = 0;
				fldt[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7j5
				fldi[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7n1
				flds[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7n1
			}
			}
		}

		// アニメ終了
		if(statc[player * 10] == 132) {
			if(repversw >= 33) edrec[player] = 125;
			else edrec[player] = 123;
			if(ending[player] ==1) {
				// TOMOYOクリア #1.60c7l9
				if(gameMode[player] >= 6) {
					stat[player] = 13;
					ending[player] = 3;
					if((gameMode[player] == 6) && (stage[player] == 44) && (!t_training[player])){
						// スタッフロール開始
						if(repversw >= 40){
							sp[player] = 1200;
							wait1[player] = wait1_master_rollG4;
							wait2[player] = wait2_master_rollG4;
							wait3[player] = wait3_master_rollG4;
							waitt[player] = waitt_master_rollG4;
							isblind[player] = 1;
						}
						ending[player] = 2;
						// エンディングBGM再生
						PlayWave(56);
fadelv[player] = 0;
					}
					else if((gameMode[player] == 8) && (mission_end[c_mission ] < 4))
						PlaySE(36);
					else if(gameMode[player] != 8)
						PlaySE(29);
					else{	//(mission_end[c_mission ] == 3)
						// スタッフロール開始
						ending[player] = 2;
						if(mission_end[c_mission] >= 5)
							hidden[player] = 8;
						if(mission_end[c_mission] == 6){
							if(repversw >= 57){
								sp[player] = speed_devil_roll;
								wait1[player] = wait1_devil_roll;
								wait2[player] = wait2_devil_roll;
								wait3[player] = wait3_devil_roll;
								waitt[player] = waitt_devil_roll;
							}else{
								sp[player] = speed_devil_roll;
								wait1[player] = wait1_devil_roll56;
								wait2[player] = wait2_devil_roll56;
								wait3[player] = wait3_devil_roll56;
								waitt[player] = waitt_devil_roll56;
							}
							hidden[player] = 8;
							IsBig[player] = 1;
						}
						// エンディングBGM再生
						PlayWave(56);
fadelv[player] = 0;
					}
				} else {

					// スタッフロール開始fastroll[player]
					ending[player] = 2;
					// エンディングBGM再生
						PlayWave(56);
fadelv[player] = 0;
//sprintf(string[0],"-3- %2d %2d PLAY56",ending[player],gameMode[player]);
//printFont(0, 4, string[0], (count % 4 / 2) * digitc[rots[i]]);
					if((repversw<=23)||(enable_grade[player]!=1)){
						if((gameMode[player] >= 1) && (gameMode[player] <= 2) && (hidden[player] != 8)) {
							if((enable_grade[player]==4)&&(grade[player]>=23)){
								hidden[player]=10;//ちょっと早く消える
							}
							shadow_timer_flag[player] = p_shadow_timer;
							semigmflag_r[player] = 1;//見えるロール
						}

						// 消えロール (移転 #1.60c7p9ex改造)
						if((shadow_heboris) && (gmflag_r[player])) hidden[player] = 8;
					}

				}
				scanItem(player);
			} else {
				// 足切り
				ending[player] = 5;
			}
			statc[player * 10] = 0;
		}

		statc[player * 10]++;
	}
	else if ( ending[player] == 2 )//ending==1ですでに消えロールなどの色々な設定をしてある
	{
fadelv[player] = 0;
		bgmteisiflg = 0;
		stat[player] = 4;
		if(repversw >= 54) statBlock(player);
	}
	else if ( ending[player] == 3 )
	{
		statc[player * 10]++;
		hidden[player] = 0;	// シャドウロール解除 #1.60c7h8
		shadow_timer_flag[player] = 0;	// #1.60c7j5
		if(stopmirror_flag[player]) fldMirrorProc(player);
		hiddeny[player] = 22;

		// 花火
		if((statc[player * 10] % 9 == 0)&&(endingcnt[player] < 30)) {//30発上がるとやめる
			PlaySE(35);
			objectCreate2(player, 7, Rand(80) + 72 + 192 * player - 96 * maxPlay, 16 + Rand(24), 0, 0, Rand(7)+1, 0);
			hanabi_total[player]++;
			endingcnt[player]++;
		}
		if(gameMode[player] != 0)StopAllBGM();
		if(gameMode[player] == 8) {
			// ミッションモード用
			ExBltRect(3, 105 + 192 * player - 96 * maxPlay, 76, 195, 230, 109, 20);
		} else if((grade[player] == 32)||
			((gameMode[player] == 7) && (anothermode[player] == 2))) {
			// YOU ARE GRAND MASTER!!
			// GMになっている場合のみ #1.60c7k3
			if(statc[player * 10] < 64) {
				k = 128 - statc[player * 10];
				i = (80 * k / 64 - 80) / 2;
				j = (36 * k / 64 - 36) / 2;
				k = 65536 * k / 64;
				ExBltRectR(3, 120 - i + 192 * player - 96 * maxPlay, 102 - j, 208, 126, 80, 36, k, k);
			}
			else{
				ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 102, 208, 126, 80, 36);
			}
		} else if((gameMode[player] == 3) && (grade[player] == 16) ){
			// YOU ARE GOD
			if(statc[player * 10] < 64) {
				k = 128 - statc[player * 10];
				i = (112 * k / 64 - 112) / 2;
				j = (66 * k / 64 - 66) / 2;
				k = 65536 * k / 64;
				ExBltRectR(3, 105 - i + 192 * player - 96 * maxPlay, 70 - j, 191, 163, 112, 66, k, k);
			}
			else{
				ExBltRect(3, 105 + 192 * player - 96 * maxPlay, 70, 191, 163, 112, 66);
			}
		} else {
			// GMになっていない場合 #1.60c7k3

			// EXCELLENT
			if(statc[player * 10] <= 5){
				k = 10 - statc[player * 10];
				i = (75 * k / 5 - 75) / 2;
				j = (9 * k / 5 - 9) / 2;
				k = 65536 * k / 5;
				ExBltRectR(3, 122 - i + 192 * player - 96 * maxPlay, 96 - j, 212, 73, 75, 9, k, k);
			} else {
				ExBltRect(3, 122 + 192 * player - 96 * maxPlay, 96, 212, 73, 75, 9);
			}

			// DEVIL-
			if((gameMode[player] == 3) && (devil_minus[player]==1)){
				printSMALLFont(130 + 192 * player - 96 * maxPlay, 157, "Try Next", 0);
				ExBltRect(86, 130 + 192 * player - 96 * maxPlay, 166, 0, 27, 72, 9);	// DEVIL-DOOM
				printSMALLFont(130 + 192 * player - 96 * maxPlay, 175, "   Mode", 0);
			}
			// ゲームモードの表示
			ExBltRect(86, 125 + 192 * player - 96 * maxPlay, 110, 72*getModeAdd(player), 99 + (9*gameMode[player]), 72, 9);
			printFont(18 + 24 * player - 12 * maxPlay, 15, "MODE", 0);
			printFont(15 + 24 * player - 12 * maxPlay, 16, "ALL CLEAR", 0);

			// TOMOYOで完全クリアしてない（27面より前止まり）なら？をつける
			if( (gameMode[player] == 6) && (stage[player] < 26) )
				printFont(24 + 24 * player - 12 * maxPlay, 16, "?", 0);
			else
				printFont(24 + 24 * player - 12 * maxPlay, 16, "!", 0);

			if(((gameMode[player] == 1) || (gameMode[player] == 2)) && (enable_grade[player]!=4)  && (examination[player]==0)){
				ExBltRect(85, 126 + 192 * player - 96 * maxPlay, 157, 0,15*7, 64, 14);//TRY AGAIN云々の文字画像
			}
		}

		if(statc[player * 10] >= 450) {
			setGameOver(player);
			fadelv[player] = 100;
			hanabi_waiting[player] = 0;
			statc[player * 10] = 21;
			statc[player * 10 + 1] = 140;
			if(checkFieldTop(player) != 22){	//フィールドが空の場合
				statc[player * 10 + 1] = 0;
				statc[player * 10] = 0;
			}
		}
	}
	else if ( ending[player] == 5 )		// とりカンを追加1.60c7g7
	{
		StopAllBGM();
		statc[player * 10]++;
		if(statc[player * 10] < 220) {
			if(gameMode[player] == 8)
				ExBltRect(3, 105 + 192 * player - 96 * maxPlay, 76, 195, 250, 109, 20);
			else{
				// EXCELLENT
				if(statc[player * 10] <= 5){
					k = 10 - statc[player * 10];
					i = (75 * k / 5 - 75) / 2;
					j = (9 * k / 5 - 9) / 2;
					k = 65536 * k / 5;
					ExBltRectR(3, 122 - i + 192 * player - 96 * maxPlay, 96 - j, 212, 73, 75, 9, k, k);
				} else {
					ExBltRect(3, 122 + 192 * player - 96 * maxPlay, 96, 212, 73, 75, 9);
				}
			}
			if((statc[player * 10] % 40 == 0)&&(endingcnt[player] < 4)) {
				PlaySE(35);
				objectCreate2(player, 7, Rand(80) + 72 + 192 * player - 96 * maxPlay, 32 + Rand(20), 0, 0, Rand(7)+1, 0);
			}
		} else {
			if(statc[player * 10] == 220) objectClearPl(player);	// Tiっぽく花火を消す
			if(gameMode[player] == 8)
				ExBltRect(3, 105 + 192 * player - 96 * maxPlay, 76, 195, 250, 109, 20);
			else
				ExBltRect(3, 122 + 192 * player - 96 * maxPlay, 96, 212, 73, 75, 9);

			printFont(16 + 24 * player - 12 * maxPlay, 14, "BUT...", 0);
			if(gameMode[player] != 8){
			printFont(16 + 24 * player - 12 * maxPlay, 15, "LET'S GO", 0);
			printFont(16 + 24 * player - 12 * maxPlay, 16, "BETTER", 0);
			printFont(16 + 24 * player - 12 * maxPlay, 17, "NEXT TIME", 0);
			}
			else{
			printFont(16 + 24 * player - 12 * maxPlay, 15, "LAST", 0);
			printFont(16 + 24 * player - 12 * maxPlay, 16, "ERASE", 0);
			printFont(16 + 24 * player - 12 * maxPlay, 17, "2 LINES", 0);
			}
			// ボタンでスキップ
			if(getPushState(player, 4)) {
				statc[player * 10] = 420;
			}
			if(statc[player * 10] >= 420) {
				setGameOver(player);
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 1] = 140; 	// gameover時の地形の灰色化処理を飛ばす
				statc[player * 10] = 21;
				if(checkFieldTop(player) != 22){	//DEVIL-
					statc[player * 10 + 1] = 0;
					statc[player * 10] = 0;
				}
			}
		}
	}else if ( ending[player] == 6 ){//そのままロール突入
		if(repversw <= 49){
			ending[player] =1;
		}
		if((gameMode[player]!=1) || ((hebo_plus[player]) && (repversw >= 58))){//MASTER以外高速化
			fastroll[player]=1;
		}
		if(!gameMode[player]){//ボーナスの文字表示
			gflash[player] = 120;
		}

		// 音楽停止 #1.60c7j9
		if((gameMode[player] != 8) || ((gameMode[player] == 8) && (mission_end[c_mission ] >= 4)))
		StopAllBGM();
		pinch[player] = 0;
		// ACEでの残り時間が少ない時の効果音も停止
		StopSE(32);

		// スタッフロール開始
		ending[player] = 2;
		// エンディングBGM再生
		if(gameMode[player] == 3){
			if(tc[player] >= 1000) bgmlv = 5;
			else bgmlv = 3;
			changeBGM(player);
		}else if(gameMode[player] == 0){
			bgmlv = 1;
			changeBGM(player);
		}else if(gameMode[player] == 5){
			bgmlv = p_bgmlv;
			changeBGM(player);
		}else
			PlayWave(56);

		statc[player * 10]++;
	}else if (ending[player] == 7){//超短縮
		// 音楽停止 #1.60c7j9
		if((gameMode[player] != 8) || ((gameMode[player] == 8) && (mission_end[c_mission ] >= 4)))
		StopAllBGM();
		if((gameMode[player] == 5) && (statc[player * 10] <= 132)) statc[player * 10] = 133;	// ブロック消去をスキップ
		pinch[player] = 0;
		// ACEでの残り時間が少ない時の効果音も停止
		StopSE(32);

		// ブロックを消す
		// BIG対応 #1.60c7j5
		if(statc[player * 10] % 6 == 0) {
			j = (fldsizeh[player] + 1) - (statc[player * 10] / 6);
			if((j >= 0) && (j <= fldsizeh[player])) {
				for(i = 0; i < fldsizew[player]; i++) {
				// ライン消しエフェクトで消える #1.60c7n5
				if( fld[i+ j * fldsizew[player] + player * 220] != 0) {
					objectCreate(player, 1, (i + 15 + 24 * player - 12 * maxPlay) * 8,(j + 3) * 8, (i - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + 1 * 250, fld[i+ j * fldsizew[player] + player * 220], 100);
				}

				fld[i+ j * fldsizew[player] + player * 220] = 0;
				fldt[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7j5
				fldi[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7n1
				flds[i+ j * fldsizew[player] + player * 220] = 0;		// #1.60c7n1
				}
			}
		}
		if((statc[player * 10] > 132)&&(statc[player * 10] < 270)){
			if(stopmirror_flag[player]) fldMirrorProc(player);
			// 花火
			if((statc[player * 10] % 9 == 0)&&(endingcnt[player] < 10)) {
				PlaySE(35);
				objectCreate2(player, 7, Rand(80) + 72 + 192 * player - 96 * maxPlay, 32 + Rand(20), 0, 0, Rand(7)+1, 0);
				hanabi_total[player]++;
				endingcnt[player]++;
			}
			// EXCELLENT
			if(statc[player * 10] <= 138){
				k = 10 - (statc[player * 10] - 133);
				i = (75 * k / 5 - 75) / 2;
				j = (9 * k / 5 - 9) / 2;
				k = 65536 * k / 5;
				ExBltRectR(3, 122 - i + 192 * player - 96 * maxPlay, 96 - j, 212, 73, 75, 9, k, k);
			} else {
				ExBltRect(3, 122 + 192 * player - 96 * maxPlay, 96, 212, 73, 75, 9);
			}
		}
						// アニメ終了
		if(statc[player * 10] == 240) {
			setGameOver(player);
			fadelv[player] = 100;
			hanabi_waiting[player] = 0;
			setGameOver(player);
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 1] = 140; 	// gameover時の地形の灰色化処理を飛ばす
			statc[player * 10] = 21;
		}

		statc[player * 10]++;
	}
//bgmteisiflg = 0;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.14 - 結果表示 #1.60c7f3
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statResult(int player) {
	int i, color, best, worst, bestT, worstT, tmp;
	statc[player * 10 + 1]++;

	// 音楽を流す #1.60c7l2
	// 2人同時で重ならないように修正 #1.60c7m1
	if( ((stat[1 - player] == 0) || (stat[1 - player] == 10)) && (!IsPlayWave(63)) && (wavebgm > 0) )
		PlayWave(63);

	//警告音が鳴っていたら止める
	StopSE(40);

	if((statc[player*10+1]==1)&&(gameMode[player]==1)&&(enable_grade[player]==4)&&(enable_randexam==2)){
		grade_pasttime[player]++;//試験の間のスパン
	}

	if((statc[player*10+1]==1)&&(enable_randexam==2)&&(enable_grade[player]==4)&&(gameMode[player]==1)){
		grade_his[4+player*5]=grade_his[3+player*5];//ここで暫定段位履歴をずらす
		grade_his[3+player*5]=grade_his[2+player*5];
		grade_his[2+player*5]=grade_his[1+player*5];
		grade_his[1+player*5]=grade_his[0+player*5];
		grade_his[0+player*5]=grade[player];
	}
/*
	// リプレイセーブ
	if((textguide) && (!playback) && (!demo)){
		if(!player) printFont(13+24 * player - 12 * maxPlay, 26, "0-9 KEY:REPLAY", 0);
		else printFont(13+24 * player - 12 * maxPlay, 26, "Q-P KEY:REPLAY", 0);
	}
	ReplaySaveCheck(player, 3);

	if(statc[player * 10 + 3]) {
		if( (sgrade[player] >= min_sgrade) || (relaymode[player])) {	//裏段位表示時に重なるので
			sprintf(string[0], "SAVED : %02d", statc[player * 10 + 3]);
			printFont(15 + 24 * player - 12 * maxPlay, 24+relaymode[player], string[0], 5);
		} else {
			printFont(15 + 24 * player - 12 * maxPlay, 20, "YOUR PLAY", 7);
			printFont(16 + 24 * player - 12 * maxPlay, 21, "WAS SAVED", 7);
			sprintf(string[0], "FILE NO.%02d", statc[player * 10 + 3]);
			printFont(15 + 24 * player - 12 * maxPlay, 23, string[0], 5);
		}
	}
*/
	printFont(15 + 24 * player - 12 * maxPlay, 6 - (2 * relaymode[player]), "PLAY DATA", 4);

	if((gameMode[player] == 9) && (relaymode[player])){	// ROTリレー
		printFont(15 + 24 * player - 12 * maxPlay, 5, "HEBORIS", fontc[0]);
		printFont(15 + 24 * player - 12 * maxPlay, 7, "TI-ARS",  fontc[1]);
		printFont(15 + 24 * player - 12 * maxPlay, 9, "TI-WORLD",fontc[2]);
		printFont(15 + 24 * player - 12 * maxPlay, 11, "ACE-SRS", fontc[3]);
		printFont(15 + 24 * player - 12 * maxPlay, 13, "ACE-ARS", fontc[4]);
		printFont(15 + 24 * player - 12 * maxPlay, 15, "ACE-ARS2",fontc[5]);
		printFont(15 + 24 * player - 12 * maxPlay, 17, "DS-WORLD",fontc[6]);
		printFont(15 + 24 * player - 12 * maxPlay, 19, "SRS-X",   fontc[7]);
		printFont(15 + 24 * player - 12 * maxPlay, 21, "D.R.S",   fontc[8]);

		bestT = relaydata[player * 9 + 0];
		worstT = relaydata[player * 9 + 0];
		best = 0;
		worst = 0;

		for(i = 0;i < 9;i++){
			if(std_opt[player] == 0){	//time
				if(relaydata[player * 9 + best] > relaydata[player * 9 + i]){
					bestT = relaydata[player * 9 + i];
					best = i;
				}else if(relaydata[player * 9 + worst] < relaydata[player * 9 + i]){
					worstT = relaydata[player * 9 + i];
					worst = i;
				}
			}else{	//lines
				if(relaydata[player * 9 + best] < relaydata[player * 9 + i]){
					bestT = relaydata[player * 9 + i];
					best = i;
				}else if(relaydata[player * 9 + worst] > relaydata[player * 9 + i]){
					worstT = relaydata[player * 9 + i];
					worst = i;
				}
			}
		}
		for(i = 0;i < 9;i++){
			best = (relaydata[player * 9 + i] == bestT);
			worst = (relaydata[player * 9 + i] == worstT);
			color = 0;
			if(best) color = (count % 4 / 2) * 7;
			if(worst) color = (count % 4 / 2) * 2;

			if(std_opt[player] == 0){
				getTime(relaydata[player * 9 + i]);
				printFont(17 + 24 * player - 12 * maxPlay, 6 + (2 * i), string[0], color);
			}else {
				sprintf(string[0],"%3dLINES",relaydata[player * 9 + i]);
				printFont(17 + 24 * player - 12 * maxPlay, 6 + (2 * i), string[0], color);
			}
		}
		if(std_opt[player] == 0){
			printFont(15 + 24 * player - 12 * maxPlay, 23, "TOTAL TIME",   4);
			getTime(time[player]);
		}else{
			printFont(15 + 24 * player - 12 * maxPlay, 23, "TOTAL LINES",   4);
			sprintf(string[0],"%3dLINES" , li[player]);
		}
		printFont(17 + 24 * player - 12 * maxPlay, 24, string[0], count % 9);


	}else if(gameMode[player] == 8) {
		// MISSIONモード #1.60c7s4
		// MISSION
		ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 65, 220, 84, 31, 7);
		sprintf(string[0], "%4d", clear_mission);
		printSMALLFont(176 + 192 * player - 96 * maxPlay, 65, string[0], 0);
		//TIME
		ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 85, 180, 119, 19, 7);
		getTime(time[player]);
		printSMALLFont(152 + 192 * player - 96 * maxPlay, 85, string[0], 0);
	} else {
		if((gameMode[player] == 7)||(gameMode[player] == 9)) {
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 65, 154, 119, 23, 7);
			sprintf(string[0], "%4d", li[player]);
			printSMALLFont(176 + 192 * player - 96 * maxPlay, 65, string[0], 0);
		} else if(!((gameMode[player] == 0) && (novice_mode[player])) ){
			// SCORE
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 65, 154, 112, 26, 7);
			sprintf(string[0], "%7d", sc[player]);
			printSMALLFont(158 + 192 * player - 96 * maxPlay, 65, string[0], 0);
		}
		// LEVEL
		ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 85, 180, 112, 26, 7);
		sprintf(string[0], "%4d", lv[player]);
		printSMALLFont(176 + 192 * player - 96 * maxPlay, 85, string[0], 0);

		// TIME
		ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 105, 180, 119, 19, 7);
		getTime(time[player]);
		printSMALLFont(152 + 192 * player - 96 * maxPlay, 105, string[0], 0);


		// GRADE #1.60c7m9
		if((gameMode[player] != 0)&& (gameMode[player] < 6)) {
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 125, 228, 119, 26, 7);

			if(gameMode[player] != 3) {
				// 非DEVIL
				if(grade[player] == 32) {
					if(enable_grade[player]==1){
					//GM
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 443 ,((count % 4 / 2)*216)+180,90,32);
					}else{
					// Gm
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 270 ,((count % 4 / 2)*216)+120,90,48);
					}
				}else if( grade[player] >= 28 ) {
					// MK-MM
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 270 ,((count % 4 / 2)*216)+((grade[player]-28)*24),90,24);
				}  else if( grade[player] == 27 ) {
					if((enable_grade[player] == 2)||(enable_grade[player] == 4)){
						//Master
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 270 ,((count % 4 / 2)*216),90,24);
					}else{
						//m
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135,443 ,((count % 4 / 2)*216),90,24);
					}
				} else if( grade[player] >= 18 ) {
					// m1～m9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 180 ,((count % 4 / 2)*216)+((grade[player]-18)*24),90,24);
				} else if( grade[player] >= 9 ) {
					// S1～S9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 90 ,((count % 4 / 2)*216)+((grade[player]-9)*24),90,24);
				} else {
					// 9～1
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 0 ,((count % 4 / 2)*216)+(grade[player]*24),90,24);
				}
			} else {
				// DEVIL
				if( grade[player] == 0 ) {
					// 1
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 0 ,((count % 4 / 2)*216)+192,90,24);
				} else {
					if(grade[player] < 10) { /* S1～S9 */
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 90 ,((count % 4 / 2)*216)+((grade[player]-1)*24),90,24);
					} else if(grade[player] < 14)  { /* S10～S13 */
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 360 ,((count % 4 / 2)*216)+((grade[player]-10)*24),90,24);
					} else if(grade[player] == 14) { /* m */
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135,443 ,((count % 4 / 2)*216),90,24);
					} else if(grade[player] == 15) { /* Gm */
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 135, 270 ,((count % 4 / 2)*216)+120,90,48);
					} else if(grade[player] == 16) { /* ネ申 */
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 130, 360 ,((count % 4 / 2)*216)+96,90,48);
					}
				}
			}
		}

		if(gameMode[player] == 0){
			if(!novice_mode[player]){
				printSMALLFont(152 + 192 * player - 96 * maxPlay, 157, "TAMAYA",(count % 4 / 2) * digitc[rots[player]]);
				sprintf(string[0], "%4d", hanabi_total[player]);
				printBIGFont(128 + 192 * player - 96 * maxPlay, 139, string[0], (count % 4 / 2) * digitc[rots[player]]);
			}else{
				ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 125, 154, 112, 26, 7);
				printSMALLFont(152 + 192 * player - 96 * maxPlay, 157, "Points",(count % 4 / 2) * digitc[rots[player]]);
				sprintf(string[0], "%6d", sc[player]);
				printBIGFont(122 + 192 * player - 96 * maxPlay, 139, string[0], (count % 4 / 2) * digitc[rots[player]]);
			}
		}

		// S-GRADE #1.60c7o1
		if( sgrade[player] >= min_sgrade ) {
			tspin_c[player] = 0;
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 160, 220, 91, 31, 7);
			ExBltRect(3, 152 + 192 * player - 96 * maxPlay, 160, 228, 119, 26, 7);
			if( sgrade[player] == 18 ) {
				// GM
				ExBltRect(26,115 + 192 * player -96 * maxPlay , 168, 270 ,((count % 4 / 2)*216)+168,90,24);
			} else if( sgrade[player] >= 9 ) {
				// S1～S9
				ExBltRect(26,115 + 192 * player -96 * maxPlay , 168, 90 ,((count % 4 / 2)*216)+((sgrade[player]-9)*24),90,24);
			} else {
				// 9～1
				ExBltRect(26,115 + 192 * player -96 * maxPlay , 168, 0 ,((count % 4 / 2)*216)+(sgrade[player]*24),90,24);
			}
		}
	}

	// ボタンでスキップ
	if(getPushState(player, 4)) {
		statc[player * 10 + 1] = 1000;
	}

	// 終了
	if(statc[player * 10 + 1] >= 500 + (300 * (relaymode[player]))) {

	if(!playback){
		stat[player] = 38;
	}else{
		if( (stat[1 - player] == 0) || (stat[1 - player] == 10) )
			StopAllBGM();		// 音楽停止 #1.60c7l3
		stat[player] = 21;	// ゲームオーバー表示 #1.60c7p1
	}

		statc[player * 10] = 0;
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		statc[player * 10 + 5] = 0;
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.38 - リプレイ保存画面
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：番号
// statc[player * 10 + 1]：リプレイの有無
// statc[player * 10 + 2]：保存入力をしたか
void statReplaySave(int player) {
	int i;

	// リプレイの存在を調べる
	if(!statc[player * 10 + 0]) {
		statc[player * 10 + 1] = loadReplayData2(player,1);
		statc[player * 10 + 0] = 1;
	}

	printFont(16 + 24 * player - 12 * maxPlay, 5, "SAVE", 4);
	printFont(16 + 24 * player - 12 * maxPlay, 6, " REPLAY?", 4);


	if((time2[player] > 72000 - 36000 * (gameMode[player] == 4)) || (!replay_save[player])){	//リプレイ記録不可
		printFont(15 + 24 * player - 12 * maxPlay, 12, "THIS PLAY", 2);
		printFont(15 + 24 * player - 12 * maxPlay, 13, "  CAN'T BE", 2);
		printFont(15 + 24 * player - 12 * maxPlay, 14, "     SAVED", 2);
		if((time2[player] > 72000) && (gameMode[player] != 4)) printFont(15 + 24 * player - 12 * maxPlay, 16, "OVER 20MIN", 2);
		if((time2[player] > 36000) && (gameMode[player] == 4)) printFont(15 + 24 * player - 12 * maxPlay, 16, "OVER 10MIN", 2);
	}else if(statc[player * 10 + 1] != 0){	//空
		if(statc[player * 10 + 2] == 0) printFont(15 + 24 * player - 12 * maxPlay, 8, " <      >", count % 9);
		sprintf(string[0],"   NO.%d",statc[player * 10 + 0]);
		printFont(15 + 24 * player - 12 * maxPlay, 8, string[0], 0);
		printSMALLFont(142 + 192 * player - 96 * maxPlay, 127, "Empty", 0);
	}else{	//既にリプレイあり
		if(statc[player * 10 + 2] == 0) printFont(15 + 24 * player - 12 * maxPlay, 8, " <      >", count % 9);
		sprintf(string[0],"   NO.%d",statc[player * 10 + 0]);
		printFont(15 + 24 * player - 12 * maxPlay, 8, string[0], 0);

		sprintf(string[0],"    VER:%d",repdata[3 + player * 20]);	//バージョン
		printFont(15 + 24 * player - 12 * maxPlay, 9, string[0], 0);

		ExBltRect(85, 120 + 192 * player - 96 * maxPlay, 77, 0, 98, 22, 7);	//モード
		ExBltRect(86, 125 + 192 * player - 96 * maxPlay, 87, 72*repdata[1 + player * 20], 99 + (9*repdata[0 + player * 20]), 72, 9);

		ExBltRect(85, 120 + 192 * player - 96 * maxPlay, 105, 35, 42, 22, 7);	//Rots
		if(repdata[2 + player * 20] != 9)
			ExBltRect(55,128 + 8 * (repdata[0 + player * 20] == 4) + 192 * player -96 * maxPlay , 113 - (4 * (getDrawRate() == 1)), 64*repdata[2 + player * 20] ,
					32*fontc[repdata[2 + player * 20]] + (7 * (getDrawRate() == 1)),64,12 + (5 * (getDrawRate() == 1)));
		else
			printFont(15 + 24 * player - 12 * maxPlay, 15, "   RANDOM", 0);

		if(repdata[0 + player * 20] == 6){	//TOMOYO
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 131, 228, 112, 26, 7);
			sprintf(string[0], "%6d", repdata[4 + player * 20] + 1);
			printSMALLFont(158 + 192 * player - 96 * maxPlay, 131, string[0], 0);

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 147, 180, 119, 19, 7);
			getTime(repdata[5 + player * 20]);
			printSMALLFont(146 + 192 * player - 96 * maxPlay, 147, string[0], 0);
		}else if(repdata[0 + player * 20] == 4){	//VS
			printFont(15 + 24 * player - 12 * maxPlay, 14, "1P", 0);
			printFont(15 + 24 * player - 12 * maxPlay, 17, "2P", 0);

			if(repdata[4 + player * 20] != 9)
				ExBltRect(55,128 + 8 + 192 * player -96 * maxPlay , 137 - (4 * (getDrawRate() == 1)), 64*repdata[4 + player * 20] ,
						32*fontc[repdata[4 + player * 20]] + (7 * (getDrawRate() == 1)),64,12 + (5 * (getDrawRate() == 1)));
			else
				printFont(15 + 24 * player - 12 * maxPlay, 18, "   RANDOM", 0);

			sprintf(string[0], "%2d POINT", repdata[5 + player * 20]);
			printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], 0);
			printFont(15 + 24 * player - 12 * maxPlay, 21, "    MATCH", 0);

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 179, 180, 119, 19, 7);
			getTime(repdata[6 + player * 20]);
			printSMALLFont(146 + 192 * player - 96 * maxPlay, 179, string[0], 0);
		}else if(repdata[0 + player * 20] == 7){	//ACE
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 131, 180, 112, 26, 7);
			sprintf(string[0], "%6d", repdata[4 + player * 20]);
			printSMALLFont(158 + 192 * player - 96 * maxPlay, 131, string[0], 0);

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 147, 154, 119, 23, 7);
			sprintf(string[0], "%6d", repdata[5 + player * 20]);
			printSMALLFont(158 + 192 * player - 96 * maxPlay, 147, string[0], 0);

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 163, 180, 119, 19, 7);
			getTime(repdata[6 + player * 20]);
			printSMALLFont(146 + 192 * player - 96 * maxPlay, 163, string[0], 0);
		}else if(repdata[0 + player * 20] == 8){	//MISSION
			getRoadName(repdata[4 + player * 20]);
			printFont(14 + 24 * player - 12 * maxPlay, 17, string[0], 0 );

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 147, 180, 119, 19, 7);
			getTime(repdata[5 + player * 20]);
			printSMALLFont(146 + 192 * player - 96 * maxPlay, 147, string[0], 0);
		}else if(repdata[0 + player * 20] == 9){	//SIMPLE
			ExBltRect(85, (18 + 24 * player - 12 * maxPlay)*8, 96, 0, 7*(25 + repdata[4 + player * 20]), 50, 7);
			if(repdata[4 + player * 20] > 1){
				ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 131, 154, 112, 26, 7);
				sprintf(string[0], "%6d", repdata[5 + player * 20]);
				printSMALLFont(158 + 192 * player - 96 * maxPlay, 131, string[0], 0);
			}
			if(repdata[4 + player * 20] != 0){
				ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 147, 154, 119, 23, 7);
				sprintf(string[0], "%6d", repdata[6 + player * 20]);
				printSMALLFont(158 + 192 * player - 96 * maxPlay, 147, string[0], 0);
			}
			if(repdata[4 + player * 20] != 1){
				ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 163, 180, 119, 19, 7);
				getTime(repdata[7 + player * 20]);
				printSMALLFont(146 + 192 * player - 96 * maxPlay, 163, string[0], 0);
			}
		}else{		//他(BEGINNER～DEVIL、ORIGINAL)
			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 131, 180, 112, 26, 7);
			sprintf(string[0], "%6d", repdata[4 + player * 20]);
			printSMALLFont(158 + 192 * player - 96 * maxPlay, 131, string[0], 0);

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 147, 180, 119, 19, 7);
			getTime(repdata[5 + player * 20]);
			printSMALLFont(146 + 192 * player - 96 * maxPlay, 147, string[0], 0);

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 163, 154, 112, 26, 7);
			sprintf(string[0], "%6d", repdata[6 + player * 20]);
			printSMALLFont(158 + 192 * player - 96 * maxPlay, 163, string[0], 0);

			ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 179, 154, 119, 23, 7);
			sprintf(string[0], "%6d", repdata[7 + player * 20]);
			printSMALLFont(158 + 192 * player - 96 * maxPlay, 179, string[0], 0);
		}
		if(repdata[9 + player * 20]) ExBltRect(85, 136 + 192 * player - 96 * maxPlay, 96, 154, 112, 14, 7);
	}
	if(statc[player * 10 + 2] == 0){
		printFont(16+24 * player - 12 * maxPlay, 25, "B:NO SAVE", 0);

		// キー入力
		padRepeat(player);
		// ←
		if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || getPressState(player, 6) )
		if( getPressState(player, 2) ) {
			PlaySE(3);
			statc[player * 10 + 0]--;
			if(statc[player * 10 + 0] < 1) statc[player * 10 + 0] = 40;
			statc[player * 10 + 1] = loadReplayData2(player,statc[player * 10 + 0]);
		}
		// →
		if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || getPressState(player, 6) )
		if( getPressState(player, 3) ) {
			PlaySE(3);
			statc[player * 10 + 0]++;
			if(statc[player * 10 + 0] > 40) statc[player * 10 + 0] = 1;
			statc[player * 10 + 1] = loadReplayData2(player,statc[player * 10 + 0]);
		}
		// A (SAVE)
		if( getPushState(player, 4) ) {
			if((time2[player] > 72000 - 36000 * (gameMode[player] == 4)) || (!replay_save[player])){
				statc[player * 10 + 2] = 120;
			}else{
				if(abs_YGS2K(GetRealFPS() - max_fps_2) < 10){
					PlaySE(10);
					saveReplayData(player, statc[player * 10 + 0]);
					statc[player * 10 + 2]++;
					statc[player * 10 + 1] = loadReplayData2(player,statc[player * 10 + 0]);
				}
			}
		}
		// B (NO SAVE)
		if( getPushState(player, 5) ) {
			statc[player * 10 + 2] = 120;
		}
	}else{
		statc[player * 10 + 2]++;
		if(statc[player * 10 + 2] < 120){
			printFont(18 + 24 * player - 12 * maxPlay, 25, "SAVED", (count % 4 / 2) * digitc[rots[player]]);
		}else{
			if( (stat[1 - player] == 0) || (stat[1 - player] == 10) )
				StopAllBGM();		// 音楽停止 #1.60c7l3
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 3] = 0;
			statc[player * 10 + 4] = 0;
			statc[player * 10 + 5] = 0;
			if(gameMode[player] == 4){	// VS
				// wait値をバックアップから戻す #1.60c7o9
				wait1[0] = b_wait1[0];
				wait2[0] = b_wait2[0];
				wait3[0] = b_wait3[0];
				waitt[0] = b_waitt[0];
				sp[0]    = b_sp[0];

				wait1[1] = b_wait1[1];
				wait2[1] = b_wait2[1];
				wait3[1] = b_wait3[1];
				waitt[1] = b_waitt[1];
				sp[1]    = b_sp[1];
				enterVersusMode(0);		// VSメニューへ
				return;
			}else{
				stat[player] = 21;	// ゲームオーバー表示 #1.60c7p1
			}
		}
	}

}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.16 - 対戦モード設定画面 #1.60c7g1
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statVersusSelect(int player) {
	int	i;
	if(demo){	//デモならタイトルへ
		StopAllBGM();
		StopSE(40);
		cpu_flag[0] = 0;
		cpu_flag[1] = 0;
		gameMode[0] = 0;
		gameMode[1] = 0;
		rots[0] = setupBak[4] % 10;
		rots[1] = setupBak[4] / 10;
		if(!tmp_maxPlay)
			maxPlay = 0;
		flag = 1;
		return;
	}

	if(hidden[player] < 0) hidden[player] = 0;
	printFont(3 + 24 * player, 5, "SELECT", 4);

	printFont(3 + 24 * player, 7, "ROT. ", 6);
	ExBltRect(55,158 + 192 * player -96 * maxPlay , 40, (64*rots[player]) ,384,64,21);

	if(vslevel[player] == 2)
		printFont(3 + 24 * player, 6, "A:LOAD SP", 7);
	else
		printFont(3 + 24 * player, 6, "A:READY", 7);

	// 回転法則
	if(versus_rot[player] == 0)
		printFont(6 + 24 * player, 8, "HEBORIS",     count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 1)
		printFont(7 + 24 * player, 8, "TI-ARS",      count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 2)
		printFont(5 + 24 * player, 8, "TI-WORLD",    count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 3)
		printFont(6 + 24 * player, 8, "ACE-SRS",   count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 4)
		printFont(6 + 24 * player,8, "ACE-ARS",     count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 5)
		printFont(5 + 24 * player, 8, "ACE-ARS2",     count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 6)
		printFont(5 + 24 * player, 8, "DS-WORLD",   count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 7)
		printFont(8 + 24 * player, 8, "SRS-X",   count % 9 * (vslevel[player] == 0));
	else if(versus_rot[player] == 8)
		printFont(8 + 24 * player, 8, "D.R.S",   count % 9 * (vslevel[player] == 0));

	else
		printFont(7 + 24 * player, 8, "RANDOM",      count % 9 * (vslevel[player] == 0));

	// せり上げタイプ
	printFont(3 + 24 * player, 9, "UPLINE", 6);
	if(upLineT[player] == 0)
		printFont(3 + 24 * player, 10, "COPYBOTTOM", count % 9 * (vslevel[player] == 1));
	else if(upLineT[player] == 1)
		printFont(8 + 24 * player, 10, "HEBO+",    count % 9 * (vslevel[player] == 1));
	else if(upLineT[player] == 2)// 旧式せり上がり#1.60c7g2
		printFont(4 + 24 * player, 10, "DS-RANDOM",  count % 9 * (vslevel[player] == 1));
	else
		printFont(3 + 24 * player, 10, "MOVEERASED",  count % 9 * (vslevel[player] == 1));

	// スピード設定
	printFont(3 + 24 * player, 11, "SP PRESET", 6);

	//速度テンプレート
	if(sptmp[player] == -10){
		printFont(3 + 24 * player, 12, "OOBAKA(!)",   count % 9 * (vslevel[player] == 2));
	}else if(sptmp[player] < 0){
		printFont(3 + 24 * player, 12, waitname[0],     count % 9 * (vslevel[player] == 2));
	}else{
		printFont(3 + 24 * player, 12, waitname[sptmp[player]],   count % 9 * (vslevel[player] == 2));
	}
	// ブロック落下スピード(1200 = 20G)
	printFont(3 + 24 * player, 13, "WAITS",   6);
	sprintf(string[0], "%7d", sp[player]);
	printFont(6 + 24 * player, 13, string[0], count % 9 * (vslevel[player] == 3));

	// WAITS 26402815
	sprintf(string[0], "W1:%2d", wait1[player]);//SYUTUGEN
	printFont(3 + 24 * player, 14, string[0], count % 9 * (vslevel[player] == 4));
	sprintf(string[0], "W2:%2d", wait2[player]);//SYOUKYO
	printFont(8 + 24 * player, 14, string[0], count % 9 * (vslevel[player] == 5));
	sprintf(string[0], "W3:%2d", wait3[player]);//SETTYAKU
	printFont(3 + 24 * player, 15, string[0], count % 9 * (vslevel[player] == 6));
	sprintf(string[0], "WT:%2d", waitt[player]);//YOKOTAME
	printFont(8 + 24 * player, 15, string[0], count % 9 * (vslevel[player] == 7));

	//それぞれ項目名表示
	if(!english){
	if(vslevel[player] == 3)
		printFont(3 + 24 * player, 16, "FALL SPEED",    8);
	else if(vslevel[player] == 4)
		printFont(3 + 24 * player, 16, "SYUTUGEN",    8);
	else if(vslevel[player] == 5)
		printFont(3 + 24 * player, 16, "SYOUKYO",     8);
	else if(vslevel[player] == 6)
		printFont(3 + 24 * player, 16, "SETTYAKU",    8);
	else if(vslevel[player] == 7)
		printFont(3 + 24 * player, 16, "YOKOTAME",   8);
	} else {
	if(vslevel[player] == 3)
		printFont(3 + 24 * player, 16, "FALL SPEED",    8);
	else if(vslevel[player] == 4)
		printFont(3 + 24 * player, 16, "ARE",    8);
	else if(vslevel[player] == 5)
		printFont(3 + 24 * player, 16, "LINE CLEAR",     8);
	else if(vslevel[player] == 6)
		printFont(3 + 24 * player, 16, "LOCK",    8);
	else if(vslevel[player] == 7)
		printFont(3 + 24 * player, 16, "DAS",   8);
	}

	if((vsmodesl==0)||(vsmodesl==3)){
		printFont(3 + 24 * player, 17, "USEITEM", 6);
		if(use_item[player] == 0){
			printFont(10 + 24 * player, 17, "ALL", count % 9 * (vslevel[player] == 8));
		}else if((use_item[player] > 0)&&(use_item[player]<=item_num)){
			sprintf(string[0], "%2d", use_item[player]);
			printFont(10 + 24 * player, 17, string[0], count % 9 * (vslevel[player] == 8));
			ExBltRect(46,(12 + 24 * player)*8, 136,(8*(use_item[player]-1))+56,0,8,8);
			ExBltRect(28,(7 + 24 * player)*8, 144, 0, (use_item[player] - 1) * 16, 48, 16);
		}else if(use_item[player] == item_num+1){
			printFont(10 + 24 * player, 17, "FEW", count % 9 * (vslevel[player] == 8));
			ExBltRect(46,(3 + 24 * player)*8, 144,56,0,40,8);
		}else if(use_item[player] == item_num+2){
			printFont(10 + 24 * player, 17, "DS", count % 9 * (vslevel[player] == 8));
			ExBltRect(46,(3 + 24 * player)*8, 144,(8*5)+56,0,8,8);
			ExBltRect(46,(4 + 24 * player)*8, 144,(8*6)+56,0,8,8);
			ExBltRect(46,(5 + 24 * player)*8, 144,(8*11)+56,0,8,8);
			ExBltRect(46,(6 + 24 * player)*8, 144,(8*12)+56,0,8,8);
			ExBltRect(46,(7 + 24 * player)*8, 144,(8*17)+56,0,8,8);
			ExBltRect(46,(8 + 24 * player)*8, 144,(8*25)+56,0,8,8);
		}else if(use_item[player] == item_num+3){
			printFont(10 + 24 * player, 17, "TGM", count % 9 * (vslevel[player] == 8));
			ExBltRect(46,(3 + 24 * player)*8, 144,56,0,40,8);
			ExBltRect(46,(8 + 24 * player)*8, 144,176,0,40,8);
			ExBltRect(46,(3 + 24 * player)*8, 152,216,0,40,8);
			ExBltRect(46,(8 + 24 * player)*8, 152,272,0,32,8);
		}
	}else{
		printFont(3 + 24 * player, 17, "NO ITEM", count % 9 * (vslevel[player] == 8));
	}

	// HIDDEN #1.60c7g2
	printFont(3 + 24 * player, 20, "WIN TYPE",6);
	if(wintype == 0){
		sprintf(string[0], "GOAL LV", 0);
		printFont(3 + 24 * player, 21, string[0], count % 9 * (vslevel[player] == 9));
	}else if(wintype == 1){
		sprintf(string[0], "GOAL LINE");
		printFont(3 + 24 * player, 21, string[0], count % 9 * (vslevel[player] == 9));
	}else if(wintype == 2){
		sprintf(string[0], "SURVIVAL");
		printFont(3 + 24 * player, 21, string[0], count % 9 * (vslevel[player] == 9));
	}

	// BIG #1.60c7g2
	printFont(3 + 24 * player, 22, "BIG", 6);
	if(IsBigStart[player])
		printFont(11 + 24 * player, 22, "ON", count % 9 * (vslevel[player] == 10));
	else
		printFont(10 + 24 * player, 22, "OFF", count % 9 * (vslevel[player] == 10));

	// ITEM #1.60c7q7 + CEMENT #1.60c7r4
	printFont(3 + 24 * player, 23, "MODE", 6);
	if(vsmodesl == 0)
		printFont(7 + 24 * player, 23, "NORMAL", count % 9 * (vslevel[player] == 11));
	else if(vsmodesl == 1)
		printFont(7 + 24 * player, 23, "NOITEM", count % 9 * (vslevel[player] == 11));
	else if(vsmodesl == 2)
		printFont(7 + 24 * player, 23, "CEMENT", count % 9 * (vslevel[player] == 11));
	else
		printFont(7 + 24 * player, 23, "NORISE", count % 9 * (vslevel[player] == 11));

	if(vs_goal == 0){
		printFont(3 + 24 * player, 24, "SURVIVAL",   6);
	}else if(wintype==0){
		// WINLV)
		printFont(3 + 24 * player, 24, "WIN LV",   6);
		if(vs_goal!=0)sprintf(string[0], "%4d", vs_goal);
		else sprintf(string[0], " NON");
		printFont(9 + 24 * player, 24, string[0], count % 9 * (vslevel[player] == 12));
	}else if(wintype==1){	// WINLINE
		printFont(3 + 24 * player, 24, "WINLINE",   6);
		if(vs_goal!=0)sprintf(string[0], "%3d", vs_goal/10);
		else sprintf(string[0], " NON");
		printFont(10 + 24 * player, 24, string[0], count % 9 * (vslevel[player] == 12));
	}else{
		printFont(3 + 24 * player, 24, "SURVIVAL",   6);
	}

	printFont(18, 5, "HOLD",  0);
	printFont(18, 6, "  +",  0);
	printFont(18, 7, "kUDn",  0);
	printFont(18, 9, "WIN",  0);
	printFont(18,10, " PTS",  0);

	// 上下移動を押しっぱなしでもできるように変更 #1.60c7l3
	padRepeat2(player);

	// ↑
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if(getPressState(player, 0)) {
		PlaySE(5);

		if(getPressState(player, 7)){
			winpoint--;
			if(winpoint <= 0) winpoint = 10;
		}else{
			vslevel[player]--;
			if(vslevel[player] < 0) vslevel[player] = 12;
		}
	}

	// ↓
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if(getPressState(player, 1)) {
		PlaySE(5);

		if(getPressState(player, 7)){
			winpoint++;
			if(winpoint > 10) winpoint = 1;
		}else{
			vslevel[player]++;
			if(vslevel[player] > 12) vslevel[player] = 0;
		}
	}

	padRepeat(player);

	// ←
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if(getPressState(player, 2)) {
		PlaySE(3);

		// 回転法則
		if(vslevel[player] == 0) {
			versus_rot[player]--;
			if(versus_rot[player] < 0) versus_rot[player] = 9;
			if(versus_rot[player] != 9) rots[player] = versus_rot[player];
			setNextBlockColors(player, 1);
		}
		// せり上げタイプ
		if(vslevel[player] == 1) {
			upLineT[player]--;
			if(upLineT[player] < 0) upLineT[player] = 3;
		}
		// 速度テンプレート
		if(vslevel[player] == 2) {
			if(sptmp[player] > -10)
				sptmp[player]--;
			else StopSE(3);
		}
		// 落下スピード
		if(vslevel[player] == 3) {
			sp[player]--;
			if(sp[player] < 0) sp[player] = 1200; // 0以下だったら1200に
		}
		// 出現
		if(vslevel[player] == 4) {
			wait1[player]--;
			if(wait1[player] < 0) wait1[player] = 99;
		}
		// 消去
		if(vslevel[player] == 5) {
			wait2[player]--;
			if(wait2[player] < 0) wait2[player] = 99;
		}
		// 接着
		if(vslevel[player] == 6) {
			wait3[player]--;
			if(wait3[player] < 0) wait3[player] = 99; // 0以下だったら99に
		}
		// 横溜め
		if(vslevel[player] == 7) {
			waitt[player]--;
			if(waitt[player] < 0) waitt[player] = 99;
		}
		// 使用するアイテム　0で全て
		if(vslevel[player] == 8) {
			use_item[player]--;
			if(use_item[player] < 0) use_item[player] = item_num+3;
		}
		// WINTYPE #1.60c7g2
		if(vslevel[player] == 9) {
			wintype--;
			if(wintype< 0) wintype= 2;
		}
		// BIG #1.60c7g2
		if(vslevel[player] == 10) {
			IsBigStart[player] = !IsBigStart[player];
		}
		// ITEM #1.60c7q7 + CEMENT #1.60c7r4
		if(vslevel[player] == 11) {
			vsmodesl--;
			if(vsmodesl < 0) vsmodesl = 3;
		}
		//vs_goal
		if(vslevel[player] == 12) {
			vs_goal = vs_goal - 50;
			if(vs_goal <= 0) vs_goal = 1000;
		}
	}

	// →
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if(getPressState(player, 3)) {
		PlaySE(3);
		// 回転法則
		if(vslevel[player] == 0) {
			versus_rot[player]++;
			if(versus_rot[player] > 9) versus_rot[player] = 0;
			if(versus_rot[player] != 9) rots[player] = versus_rot[player];
			setNextBlockColors(player, 1);
		}
		// せり上げタイプ
		if(vslevel[player] == 1) {
			upLineT[player]++;
			if(upLineT[player] > 3) upLineT[player] = 0;
		}
		// 速度テンプレート
		if(vslevel[player] == 2) {
			if(sptmp[player] < skip_fwait){
				sptmp[player]++;
				if(sptmp[player] < 0) sptmp[player] = 0;
			}else StopSE(3);
		}
		// 落下スピード
		if(vslevel[player] == 3) {
			sp[player]++;
			if(sp[player] > 1200) sp[player] = 0;
		}
		// 出現
		if(vslevel[player] == 4) {
			wait1[player]++;
			if(wait1[player] > 99) wait1[player] = 0;
		}
		// 消去
		if(vslevel[player] == 5) {
			wait2[player]++;
			if(wait2[player] > 99) wait2[player] = 0;
		}
		// 接着
		if(vslevel[player] == 6) {
			wait3[player]++;
			if(wait3[player] > 99) wait3[player] = 0; // 0以下だったら99に
		}
		// 横溜め
		if(vslevel[player] == 7) {
			waitt[player]++;
			if(waitt[player] > 99) waitt[player] = 0;
		}
		// 使用するアイテム　0で全て
		if(vslevel[player] == 8) {
			use_item[player]++;
			if(use_item[player] > item_num+3) use_item[player] = 0;
		}
		// WINTYPE #1.60c7g2
		if(vslevel[player] == 9) {
			wintype++;
			if(wintype > 2) wintype = 0;
		}
		// BIG #1.60c7g2
		if(vslevel[player] == 10) {
			IsBigStart[player] = !IsBigStart[player];
		}
		// ITEM #1.60c7q7 + CEMENT #1.60c7r4
		if(vslevel[player] == 11) {
			vsmodesl++;
			if(vsmodesl > 3) vsmodesl = 0;
		}
		if(vslevel[player] == 12) {
			vs_goal = vs_goal + 50;
			if(vs_goal > 1000) vs_goal = 50;
		}
	}

	//VSMODE
	if(vsmodesl == 0){//ノーマル
		disrise = 0;
		noitem = 0;
	}else if(vsmodesl == 1){//ノーアイテム
		noitem = 1;
		disrise = 0;
	}else if(vsmodesl == 2){//セメント
		noitem = 1;
		disrise = 1;
	}else if(vsmodesl == 3){//ノーライズ
		noitem = 0;
		disrise = 1;
	}
	//回転法則のランダムセレクト
	if(versus_rot[player] == 9){
		rots[player] = Rand(9);
		setNextBlockColors(player, 1);
	}

	// Bボタン
	if(getPushState(player, 5)) {
		upLineT[0] = uplinetype;
		use_item[0] = 0;
		upLineT[1] = uplinetype;
		use_item[1] = 0;
		gameMode[0] = 0;
		gameMode[1] = 0;
		if(!tmp_maxPlay) {
			maxPlay = 0;
		}
		setupBak[23] = wintype;
		setupBak[24] = vs_goal;
		setupBak[25] = noitem;
		setupBak[26] = disrise;
		flag = 1;
	}

	// Aボタン
	if(getPushState(player, 4)) {
		if(vslevel[player] == 2) {
			PlaySE(3);
			if(sptmp[player] == -10){
				sp[player] = 1200;
				wait1[player] = 1;
				wait2[player] = 0;
				wait3[player] = 0;
				waitt[player] = 5;
				PlaySE(18);
			}else if((sptmp[player] >= 0) && (sptmp[player] <= skip_fwait))
				loadWait(player, sptmp[player]); // 対応した設定を読み込む。
			else if(sptmp[player] > -10)
				loadWait(player, 0);
		}else{
			PlaySE(10);
			bgmlv = 10;							// vsmode.wavを鳴らす
			upLines[player] = 0;
			hiddenProc(player);
			if(IsBigStart[player]) {
				IsBig[player] = 1;
			}
			// 制限時間設定 #1.60c7r5
			if(vs_time != 0) {
				ltime[0] = vs_time;
				ltime[1] = vs_time;
			}

			// wait値をバックアップ #1.60c7l3
			b_wait1[player] = wait1[player];
			b_wait2[player] = wait2[player];
			b_wait3[player] = wait3[player];
			b_waitt[player] = waitt[player];
			b_sp[player]    = sp[player];

			stat[player] = 1;					// ブロックシャッター実行
			statc[player * 10] = 0;				// ステータスカウンタを0に
			statc[player * 10 + 1] = 10;		// シャッター後はステータスNo.3
		}
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  勝敗決定 #1.60c7g1
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void winner() {
	int		player, i, j, block, win, obj, c, kosa;

	// BGM停止
	if(wavebgm > 0) StopAllBGM();
	// 残り時間が少ない時の効果音も停止
	StopSE(32);

	timeOn[0] = 0;
	timeOn[1] = 0;
	onRecord[0] = 0;
	onRecord[1] = 0;
//	time[1] = time[0];

	winc++;
	wink++;
	statc[0]++;
	winu = winu + wink / 4;
	if(winu > 140) {
		winu = 140;
		wink = - 20;
	}

	win = (stat[1] == 7) + ((stat[0] == 7) && (stat[1] == 7));//窒息

	StopSE(40);
//	pinch[0] = 0;
//	pinch[1] = 0;

	for(player = 0; player <= 1; player++) {
		hiddeny[player] = 22;
		cpu_flag[player] = 0;
		if(winc == 0) statusClear(player);

		if(win == player)
			obj = 1;
		else if(win != 2){
			pinch[player] = 0;
			obj = 0;
		}else
			obj = 2;
		if((stat[player] != 7) && (stat[player] == 5)){
			kosa = bk[player] * 7 / (wait3[player] + (wait3[player] == 0));
			if(kosa > 6) kosa = 6;
			drawCBlock (player, blk[player] + 1, kosa, 0 , 0 , 0);
			if(spawn_y_type)// フィールド枠
				viewFldFrame(1,player);
		}
		ExBltRect(3, 22 + player * 192, winu, 96, obj * 25, 86, 24);

		if( (obj == 0) && (winr > 10) ) {
			if(wintype == 1){
				if(li[player] == vs_goal/10) {
					printFont(15 + 24 * player - 12 * maxPlay, 23, "GOAL  TIME", 7);
					getTime(time[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				} else if( ltime[player] <= 0 ) {
					printFont(16 + 24 * player - 12 * maxPlay, 22, "BY LINES", 7);
					sprintf(string[0],"1P:%5d",li[0]);
					printFont(16 + 24 * player - 12 * maxPlay, 23, string[0], 0);
					sprintf(string[0],"2P:%5d",li[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				}
			}else{
				if(tc[player] == vs_goal) {
					printFont(15 + 24 * player - 12 * maxPlay, 23, "GOAL  TIME", 7);
					getTime(time[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				} else if( ltime[player] <= 0 ) {
					printFont(16 + 24 * player - 12 * maxPlay, 22, "BY LEVEL", 7);
					sprintf(string[0],"1P:%5d",tc[0]);
					printFont(16 + 24 * player - 12 * maxPlay, 23, string[0], 0);
					sprintf(string[0],"2P:%5d",tc[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				}
			}
		}
		// Aボタンで設定画面に戻る
		if((getPushState(player, 4) && (winr > 22) && (!playback)) || (demo) && (winr > 19) || (winr > 80)) {
			if((playback) || (demo)){
				if(!tmp_maxPlay)
					maxPlay = 0;
				if(playback) restoreSetups();
				flag = 1;
				gameMode[0] = 0;
				gameMode[1] = 0;
				return;
			}
			if(cp_player_1p && cp_player_2p && (!playback)){	// ウオッチモードはリプレイ保存不可
				enterVersusMode(0);
			}else if(!playback){
				for(i = 0; i < 10; i++)
					for(j = 0; j < 22; j++) {
						fld[i + j * 10 + 0 * 220] = 0;
						fldt[i + j * 10 + 0 * 220] = 0;
						fldi[i + j * 10 + 0 * 220] = 0;
						flds[i + j * 10 + 0 * 220] = 0;
						fld[i + j * 10 + 1 * 220] = 0;
						fldt[i + j * 10 + 1 * 220] = 0;
						fldi[i + j * 10 + 1 * 220] = 0;
						flds[i + j * 10 + 1 * 220] = 0;
					}
				stat[0] = 38;	// 1Pはりプレイ保存画面へ
				statc[0] = 0;
				statc[1] = 0;
				statc[2] = 0;
				statc[3] = 0;
				statc[4] = 0;
				statc[5] = 0;
				b_to_b_c[0] = 0;
				tspin_c[0] = 0;

				stat[1] = 10; // 2Pは待機
				b_to_b_c[1] = 0;
				tspin_c[1] = 0;
			}
			flag = 0;
		}
		// 花火
		if( (statc[0] % 9 == 0) && (endingcnt[player] <= 30) && (obj == 0)) {
			PlaySE(35);
			objectCreate2(player, 7, Rand(80) + 72 + 192 * player - 96 * maxPlay, 32 + Rand(20), 0, 0, Rand(7)+1, 0);
			endingcnt[player]++;
		}
	}

	if(winc > 4) {
		winc = 0;

		block = winr;

		// 下から消去は音なし #1.60c7h3
		if((winr == 0) && (deadtype)) PlaySE(7);

		for(player = 0; player <= 1; player++) {
			if( winr <= 22	 ) {
				for(j = 0; j < 10; j++)
					if(stat[player] == 7) {
						// 死亡アニメを選べるようにした#1.60c7h3
						if(deadtype) {
							if(block <= fldsizeh[player]){
								c = fld[j + block * 10 + player * 220];
								if(c != 0) {
									if(c >= 9)
										fld[j + block * 10 + player * 220] = 10;
									else
										fld[j + block * 10 + player * 220] = 1;
								}
							}
						} else {
							// スーッと消えていく演出 #1.60c7k6
							// 対戦でも有効 #1.60c7k9
							if(21 - block >= 0) {
								c = fldt[j + (210 - (block * 10)) + player * 220];
								if((c <= -1) || (c > 8)) c = 8;
								fldt[j + (210 - (block * 10)) + player * 220] = c - 4;
							}
							if(block > 0) {
								c = fldt[j + (210 - (block - 1) * 10) + player * 220];
								fldt[j + (210 - (block - 1) * 10) + player * 220] = c - 4;
							}
						}
                    }
			} /* if( winr <= (20 + IsBig[player]) ) */
		}/* for(player = 0; player <= 1; player++) */
		winr++;
	}
}

void winner2() {
	int		player, i, j, block, win, obj, c, kosa, sbak[30];

	StopSE(32);

	timeOn[0] = 0;
	timeOn[1] = 0;
//	time[1] = time[0];

	winc++;
	wink++;
	winu = winu + wink / 4;
	if(winu > 140) {
		winu = 140;
		wink = - 20;
	}

	win = (stat[1] == 7) + ((stat[0] == 7) && (stat[1] == 7));//窒息

	StopSE(40);

	for(player = 0; player <= 1; player++) {
		padRepeat(player);
		hiddeny[player] = 22;
		cpu_flag[player] = 0;
		if(winc == 0) statusClear(player);

		if(win == player)
			obj = 1;
		else if(win != 2){
			pinch[player] = 0;
			obj = 0;
		}else
			obj = 2;
		if((stat[player] != 7) && (stat[player] == 5)){
			kosa = bk[player] * 7 / (wait3[player] + (wait3[player] == 0));
			if(kosa > 6) kosa = 6;
			drawCBlock (player, blk[player] + 1, kosa, 0 , 0 , 0);
			if(spawn_y_type)// フィールド枠
				viewFldFrame(1,player);
		}
		if(obj == 2) ExBltRect(3, 22 + player * 192, winu, 96, obj * 25, 86, 24);

		if( (obj == 0) && (winr > 10) ) {
			if(wintype == 1){
				if(li[player] == vs_goal/10) {
					printFont(15 + 24 * player - 12 * maxPlay, 23, "GOAL  TIME", 7);
					getTime(time[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				} else if( ltime[player] <= 0 ) {
					printFont(16 + 24 * player - 12 * maxPlay, 22, "BY LINES", 7);
					sprintf(string[0],"1P:%5d",li[0]);
					printFont(16 + 24 * player - 12 * maxPlay, 23, string[0], 0);
					sprintf(string[0],"2P:%5d",li[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				}
			}else{
				if(tc[player] == vs_goal) {
					printFont(15 + 24 * player - 12 * maxPlay, 23, "GOAL  TIME", 7);
					getTime(time[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				} else if( ltime[player] <= 0 ) {
					printFont(16 + 24 * player - 12 * maxPlay, 22, "BY LEVEL", 7);
					sprintf(string[0],"1P:%5d",tc[0]);
					printFont(16 + 24 * player - 12 * maxPlay, 23, string[0], 0);
					sprintf(string[0],"2P:%5d",tc[1]);
					printFont(16 + 24 * player - 12 * maxPlay, 24, string[0], 0);
				}
			}
		}
		increment_time(player);
	}

	// 次のラウンドへ
	if(winr > 30) {
		flag = 0;
	}

	if(flag == 0){	// 次のラウンドへの準備
	for(player = 0; player <= 1; player++) {
		// 初期化されては困るものをバックアップにコピー
		sbak[0]  = IsBigStart[player];
		sbak[1]  = upLineT[player];
		sbak[2]  = nextc[player];	// NEXTカウント
		sbak[3]  = next[player];	// NEXTブロック
		sbak[4]  = use_item[player];
		sbak[5]  = item_num;
		sbak[6]  = item_interval;
		sbak[7]  = replay_save[player];	// 記録可能フラグ
		sbak[8]  = time2[player];	// リプレイ用プレイタイム
		sbak[9]  = bgmlv;			// BGM
		sbak[10] = repversw;
		sbak[11] = rots[player];
		sbak[12] = sp[player];		// 各種速度
		sbak[13] = wait1[player];	//
		sbak[14] = wait2[player];	//
		sbak[15] = wait3[player];	//
		sbak[16] = waitt[player];	//
		sbak[17] = vs_round;
		sbak[18] = first_rot[player];
		sbak[19] = time[player];
		sbak[20] = vs_points[player];

		playerInitial(player);

		IsBigStart[player] = sbak[0];
		upLineT[player]    = sbak[1];
		nextc[player]      = sbak[2];	// NEXTカウント
		next[player]       = sbak[3];	// NEXTブロック
		use_item[player]   = sbak[4];
		item_num           = sbak[5];
		item_interval      = sbak[6];
		replay_save[player]= sbak[7];	// 記録可能フラグ
		time2[player]      = sbak[8];	// リプレイ用プレイタイム
		bgmlv              = sbak[9];	// BGM
		repversw           = sbak[10];	//
		rots[player]       = sbak[11];
		sp[player]         = sbak[12];	// 各種速度
		wait1[player]      = sbak[13];	//
		wait2[player]      = sbak[14];	//
		wait3[player]      = sbak[15];	//
		waitt[player]      = sbak[16];	//
		vs_round           = sbak[17];
		first_rot[player]  = sbak[18];
		if(!player) time[player]       = sbak[19];
		vs_points[player]  = sbak[20];

		b_wait1[player] = wait1[player];
		b_wait2[player] = wait2[player];
		b_wait3[player] = wait3[player];
		b_waitt[player] = waitt[player];
		b_sp[player]    = sp[player];

		if(vs_time != 0) {
			ltime[player] = vs_time;
		}

		if(IsBigStart[player])
			IsBig[player] = 1;

		item_inter[player] = item_interval;
		//回転法則のランダムセレクト
		if(versus_rot[player] == 9){
			rots[player] = rand(9,player);
		}
		setNextBlockColors(player, 1);

		stat[player] = 3;					// Ready
	}
	vs_round++;
	return;
	}

	if(winc > 4) {
		winc = 0;

		block = winr;

		// 下から消去は音なし #1.60c7h3
		if((winr == 0) && (deadtype)) PlaySE(7);

		for(player = 0; player <= 1; player++) {
			if( winr <= 22	 ) {
				for(j = 0; j < 10; j++)
					if(stat[player] == 7) {
						// 死亡アニメを選べるようにした#1.60c7h3
						if(deadtype) {
							if(block <= fldsizeh[player]){
								c = fld[j + block * 10 + player * 220];
								if(c != 0) {
									if(c >= 9)
										fld[j + block * 10 + player * 220] = 10;
									else
										fld[j + block * 10 + player * 220] = 1;
								}
							}
						} else {
							// スーッと消えていく演出 #1.60c7k6
							// 対戦でも有効 #1.60c7k9
							if(21 - block >= 0) {
								c = fldt[j + (210 - (block * 10)) + player * 220];
								if((c <= -1) || (c > 8)) c = 8;
								fldt[j + (210 - (block * 10)) + player * 220] = c - 4;
							}
							if(block > 0) {
								c = fldt[j + (210 - (block - 1) * 10) + player * 220];
								fldt[j + (210 - (block - 1) * 10) + player * 220] = c - 4;
							}
						}
                    }
			} /* if( winr <= (20 + IsBig[player]) ) */
		}/* for(player = 0; player <= 1; player++) */
		winr++;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  アイテム発動処理 #1.60c7n1
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void eraseItem(int player, int type) {
	int i, enemy, attack;
	int bx2, by2;

	if((type <= 0) || (type >= fldihardno) || (gameMode[player] > 5)) return;
	attack = 0;
	// 攻撃対象を決める
	if((gameMode[player] == 4)&&(!isreflect[1-player])) {
		enemy = 1 - player;
	} else {
		enemy = player;
	}
	if((type != 13) && (type != 17) && (type != 18) && (type != 19) && ((type != 26) || (repversw >= 65)) && (type != 27) &&
	   (type != 28) && (type != 29) && (type != 30) && (type != 32) && (type != 33) && (type != 35) &&
	   ((type != 36) || (repversw < 58))){
		attack = 1;
	}
	// 攻撃対象のアイテムを全て解除（攻撃アイテムのみ）
	if( attack ){
		statusClear(enemy);
		if((!isfever[enemy]) && (repversw >= 44)){
			item_t[enemy] = 0;
			item_timer[enemy] = 0;
		}
		if((!isfever[enemy]) && (isreflect[enemy]) && (gameMode[enemy] != 4) && (repversw >= 65) && (type != 24) && (type != 37)){
			PlaySE(41);
			objectCreate2(enemy, 10, 1, 0, 0, 2, 33, 0);
			isreflect[enemy] = 0;
			for(i = 0; i < 220; i++) {
				if((fldi[i + player * 220] < fldihardno))
					fldi[i + player * 220] = 0;
			}
			scanItem(enemy);
			return;
		}
		if(isfever[enemy]){
			PlaySE(41);
			objectCreate2(enemy, 10, 1, 0, 0, 2, 13, 0);
		}
	}

	if(!isfever[enemy]){		//攻撃アイテム（FEVER中は無効）
		if(repversw < 44){
			item_t[enemy] = 0;
			item_timer[enemy] = 0;
		}

		// MIRROR
		if( type == 1 ) {
			isfmirror[enemy] = 1;
		}
		// ROLL ROLL
		if( type == 2 ) {
			isrollroll[enemy] = 1;
			item_rollroll[enemy] = 1;
		}
		// DEATH BLOCK
		if( type == 3 ) {
			IsBig[enemy] = 1;
		}
		// X-RAY
		if( type == 4 ) {
			isxray[enemy] = 1;
			xray_flag[enemy] = 1;
		}
		// COLOR
		if( type == 5 ) {
			iscolor[enemy] = 1;
			color_flag[enemy] = 1;
		}
		//ROTATE LOCK
		if( type == 6 ) {
			isrotatelock[enemy] = 1;
			PlaySE(37);
			// NEXTブロックの向きをランダムに
			rt_nblk[0 + 6 * enemy] = rand(4,enemy);
			rt_nblk[1 + 6 * enemy] = rand(4,enemy);
			rt_nblk[2 + 6 * enemy] = rand(4,enemy);
			rt_nblk[3 + 6 * enemy] = rand(4,enemy);
			rt_nblk[4 + 6 * enemy] = rand(4,enemy);
			rt_nblk[5 + 6 * enemy] = rand(4,enemy);
			thunder_timer = 30;
			item_timer[enemy] = 600 / (1+ ((sp[enemy] >= 180) && (repversw >= 40)) );//3G以上で半減
			if(cpu_flag[enemy]) {
				cpuCheckBestSpot(enemy);
			}
			if((gameMode[enemy] == 3) && (tc[player] >= 300))
				item_timer[enemy] = 180;
		}
		//HIDE NEXT
		if( type == 7 ) {
			ishidenext[enemy] = 1;
			item_timer[enemy] = 900 - ( ((sp[enemy] >= 1200) && (repversw >= 40)) * 300);
		}
		//MAGNET BLOCK
		if( type == 8 ) {
			ismagnetblock[enemy] = 1;
		}
		//TIME STOP
		if( type == 9 ) {
			istimestop[enemy] = 1;
			PlaySE(47);
			item_timer[enemy] = 300;
			if(gameMode[enemy] == 3)
				item_timer[enemy] = 120;
		}
		//HOLD LOCK
		if( type == 10 ) {
			isholdlock[enemy] = 1;
		}
		//→← REVERSE
		if( type == 11 ) {
			isLRreverse[enemy] = 1;
			item_timer[enemy] = 600;
		}
		//BOOST FIRE
		if( type == 12 ) {
			isboost[enemy] = 1;
			item_timer[enemy] = 600 / (1+ ((sp[enemy] >= 1200) && (repversw >= 54)) );
			if((gameMode[enemy] == 3) && (tc[player] >= 300))
				item_timer[enemy] = 180 - (60 * (tc[player] >= 500));
		}

		//↑↓ REVERSE
		if( type == 14 ) {
			isUDreverse[enemy] = 1;
			item_timer[enemy] = 600;
		}
		//REMOTE CONTROL
		if( type == 15 ) {
			isremote[enemy] = 1;
		}
		//DARK
		if( type == 16 ) {
			isdark[enemy] = 1;
			item_timer[enemy] = 480;
			if((gameMode[enemy] == 3) && (tc[player] >= 300))
				item_timer[enemy] = 300;
		}

		//TRANCE FORM
		if( type == 20 ) {
			istrance[enemy] = 1;
		}

		//LASER(操作中以外)
		if( type == 21 ) {
			dolaser[enemy] = 1;
		}
		//NEGA FIELD(操作中以外)
		if( type == 22 ) {
			donega[enemy] = 1;
		}

		//SHOT GUN!(操作中以外)
		if( type == 23 ) {
			doshotgun[enemy] = 1;
		}
		//EXCHG FIELD(操作中以外)
		if( type == 24 ) {
			doexchg[enemy] = 1;
			doexchg[player] = 1;
		}
		//HARD BLOCK
		if( type == 25 ) {
			item_nblk[0 + enemy * 6] = fldihardno;
			scanItem(enemy);
		}
		//SHUFFLE FIELD
		if( type == 26 ) {
			isbanana[enemy] = 1;
		}
		//180°FIELD(操作中以外)
		if( type == 31 ) {
			do180field[enemy] = 1;
		}

		//ダブル
		if( type == 34 ) {
			isdouble[enemy] = 1;
			item_timer[enemy] = 600;
		}
		//MISS(自分に)
		if( type == 36 ) {
			ismiss[player] = 1;
			misstimer[player] = item_interval;
		}
		//COPY FIELD(操作中以外)
		if( type == 37 ) {
			doexchg[enemy] = 1;
			docopyfld[player] = 1;
		}
		//faKE NEXT
		if( type == 38 ) {
			isfakenext[enemy] = 1;
			item_timer[enemy] = 480 ;
		}
		//[] BLOCK(操作中以外)
		if( type == 39 ) {
			item_monochrome[enemy] = 1;
		}

	}	//!isfever[enemy]

	//サポートアイテム

	//FEVER!!
	if( type == 13 ) {
	// 発動者のアイテムを全て解除
		statusClear(player);
		ismiss[player] = 0;

		item_t[player] = 0;
		misstimer[player] = 0;
	// 解除ここまで
		if(!ending[player]){	//ロール中はBGMを変えない
			StopWave(50 +bgmlv);
			PlayWave(65);
		}
		isfever[player] = 1;
		item_timer[player] = 600;
		next[player] = 0;	//NEXTを棒に変化
		if(isWRule(player)){	//色も棒に変換
			c_cblk[player] = wcol[0];
			c_cblk_r[player] = wcol[0];
		} else if((rots[player] >= 4) && (rots[player] != 8)){
			c_cblk[player] = acol[0];
			c_cblk_r[player] = acol[0];
		} else{
			c_cblk[player] = ccol[0];
			c_cblk_r[player] = ccol[0];
		}
	}
	if(type == 17){
		isUPdelfield[player] = 1;
	}
	if(type == 18){
		isDWdelfield[player] = 1;
	}
	if(type == 19){
		isdeleven[player] = 1;
	}
	//RANDOM
	if( type == 27 ){
		dorulet[player] = 1;
	}
	//FREE FALL
	if(type == 28){
		isFreefall[player] = 1;
	}
	if(type == 29){
		isLmovfield[player] = 1;
	}
	if(type == 30){
		isRmovfield[player] = 1;
	}
	//16t
	if( type == 32 ) {
		do16t[player] = 1;
	}
	//REFLECT
	if( type == 33 ) {
		if(repversw >= 65){
			statusClear(player);
			item_t[player] = 0;
		}
		misstimer[player] = 0;
		isreflect[player] = 1;
		item_timer[player] = 600;
		if((gameMode[player] != 4) && (repversw >= 65))
			item_timer[player] = 1800;
	}
	if(type == 35){
		isallclear[player] = 1;
	}


	// 操作中の場合は強制的に次のブロックを出現
	if((stat[enemy] == 5) && (type != 6) && (type != 7) && (type != 11) && (type != 12)
	 && (type != 13) && (type != 14) && (type != 17) && (type != 18) && (type != 19)&& (type != 26)
	 && ((type < 27) || (type > 30)) && (!isfever[enemy])&& (type != 32)
	 && (type != 33) && (type != 34) && (type != 35) && (type != 36)) {
		// 弾け飛ぶアニメーション #1.60c7o6
		if(!((type==9) && (repversw >= 35))){
			for(i = 0; i < 4; i++) {
				if(rots[enemy] == 8) {
					bx2 = (bx[enemy] + blkDDataX[blk[enemy] * 16 + rt[enemy] * 4 + i]);
					by2 = (by[enemy] + blkDDataY[blk[enemy] * 16 + rt[enemy] * 4 + i]);
				} else if(isWRule(enemy)) {
					bx2 = (bx[enemy] + blkWDataX[blk[enemy] * 16 + rt[enemy] * 4 + i]);
					by2 = (by[enemy] + blkWDataY[blk[enemy] * 16 + rt[enemy] * 4 + i]);
				} else {
					bx2 = (bx[enemy] + blkDataX[blk[enemy] * 16 + rt[enemy] * 4 + i]);
					by2 = (by[enemy] + blkDataY[blk[enemy] * 16 + rt[enemy] * 4 + i]);
				}
				objectCreate(enemy, 6, (bx2 + 15 + 24 * enemy - 12 * maxPlay) * 8, (by2 + 3) * 8, (bx2 - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + 250, c_cblk[enemy]+1, 100);
			}
			ndelay[enemy] = 1;
			stat[enemy] = 22;
			dhold[player] = disable_hold;		// hold使用可能に
			statc[enemy * 10 + 0] = 60;		// 待ち時間
		}
		if( (type == 9) && (repversw < 35) ) {				//TIME STOP停止時間
			statc[enemy * 10 + 0] = 300;
		}
		if( type == 21) {	//LASER(操作中)
			dolaser[enemy] = 0;
			stat[enemy] = 26;
			statc[enemy * 10 + 0] = 0;
		}
		if( type == 22) {	//NEGA FIELD(操作中)
			donega[enemy] = 0;
			stat[enemy] = 27;
			statc[enemy * 10 + 0] = 0;
		}
		if( type == 23) {	//SHOT GUN!(操作中)
			doshotgun[enemy] = 0;
			stat[enemy] = 28;
			statc[enemy * 10 + 0] = 0;
		}
		if( type == 24) {	//EXCHG FIELD(操作中)
			doexchg[enemy] = 0;
			stat[player] = 29;
			stat[enemy] = 29;
			statc[enemy * 10 + 0] = 0;
		}
		if( type == 31) {	//180°FIELD(操作中)
			do180field[enemy] = 0;
			stat[enemy] = 34;
			statc[enemy * 10 + 0] = 0;
		}
		if( type == 37) {	//COPY FIELD(操作中)
			doexchg[enemy] = 0;
			//stat[player] = 38;
			stat[enemy] = 29;
			statc[enemy * 10 + 0] = 0;
			statc[player * 10 + 3] = 1;
		}
		statc[enemy * 10 + 1] = 4;		// ステータス
		statc[enemy * 10 + 2] = 0;
		statc[enemy * 10 + 3] = 0;
		statc[enemy * 10 + 4] = 0;
		bdowncnt[enemy]++;

		// アイテム飛ばし #1.60c7o6
		if(item[enemy]) {
			item[enemy] = 0;
			scanItem(enemy);
		}
	}
	if((gameMode[player] == 4) && (attack) && (isreflect[1-player])){
		objectCreate2(player, 10, 0, 0, 0, 1, 33, 0);
		PlaySE(38);
	}

	if(!isfever[enemy])
	if((type <= 5) || (type == 8) || (type == 10) || (type == 15) || (type == 16) ||
		(type == 20) || (type == 22) || (type == 23) || (type == 25) || (type == 31)||
		(type == 34)|| (type == 37)|| (type == 38)|| (type == 39)){
		if((gameMode[player] != 4) || (!isreflect[1-player]))
			objectCreate2(enemy, 10, 0, 0, 0, 0, type, 0);//WARNIGのエフェクト
		PlaySE(38);
		if(((type == 3) && (IsBigStart[enemy])) || ((type == 37) && (gameMode[player] != 4)))
			StopSE(38);
	}
	// フィールド上に残っているアイテムを全て消す #1.60c7n1
	for(i = 0; i < 220; i++) {
		if((fldi[i + player * 220] < fldihardno) || (repversw < 32))	//ハードブロックも消えていた問題を修正 C7U1.5
			fldi[i + player * 220] = 0;
	}
	scanItem(player);
}
//ステータス異常をクリア
void statusClear(int player){
	if(isfmirror[player])
		stopmirror_flag[player] = 1;
	isrollroll[player] = 0;
	item_rollroll[player] = 0;
	if(!IsBigStart[player]) {	// 始めからBIGの場合は解除しない #1.60c7r3
		IsBig[player] = 0;
	}
	isxray[player] = 0;
	xray_flag[player] = 0;
	iscolor[player] = 0;
	color_flag[player] = 0;
	if(isrotatelock[player]){
		isrotatelock[player] = 0;
		rt_nblk[0 + 6 * player] = 0;
		rt_nblk[1 + 6 * player] = 0;
		rt_nblk[2 + 6 * player] = 0;
		rt_nblk[3 + 6 * player] = 0;
		rt_nblk[4 + 6 * player] = 0;
		rt_nblk[5 + 6 * player] = 0;
		if(cpu_flag[player]) {
			cpuCheckBestSpot(player);
		}
	}
	ishidenext[player] = 0;
	ismagnetblock[player] = 0;
	isholdlock[player] = 0;
	istimestop[player] = 0;
	isLRreverse[player] = 0;
	isUDreverse[player] = 0;
	istrance[player] = 0;
	isdouble[player] = 0;
	isfakenext[player] = 0;
	isboost[player] = 0;
	isremote[player] = 0;
	isdark[player] = 0;
	item_monochrome[player] = 0;
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.21 - ゲームオーバー表示
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statGameOver2(int player) {
	int i;

	// カウンタ増加
	statc[player * 10]++;

	if( (stat[1 - player] == 0) || (stat[1 - player] == 10) ) StopAllBGM();	// 音楽停止

	// リプレイ保存
	/*
	if((textguide) && (!playback) && (!demo)){
		if(!player) printFont(13+24 * player - 12 * maxPlay, 26, "0-9 KEY:REPLAY", 0);
		else printFont(13+24 * player - 12 * maxPlay, 26, "Q-P KEY:REPLAY", 0);
	}
	ReplaySaveCheck(player, 3);

	if(statc[player * 10 + 3]) {
		printFont(15 + 24 * player - 12 * maxPlay, 20, "YOUR PLAY", 7);
		printFont(16 + 24 * player - 12 * maxPlay, 21, "WAS SAVED", 7);
		sprintf(string[0], "FILE NO.%02d", statc[player * 10 + 3]);
		printFont(15 + 24 * player - 12 * maxPlay, 23, string[0], 5);
	}
	*/
	// ゲームオーバー音
	if(statc[player * 10] == 1) PlaySE(8);
	if(statc[player * 10] >= 2) {
		ExBltRect(81, 110+192 * player -96 * maxPlay , 95 , 0, 0, 100, 50);
	}

	// ボタンでスキップ
	if(getPushState(player, 4)) {
		statc[player * 10] = 480;
	}

	// 終了
	if(statc[player * 10] >= 480) {
		ndelay[player] = 1;
		if(gameMode[player] == 8) {
			StopSE(8);
			versusInit(0);
			playerInitial(player);
			loadMissionData(mission_file);
			stat[player] = 23;		// ミッションモードの場合
			if(playback)
				stat[player] = 0;
		} else {
			if((gameMode[player] == 6) && (tmp_maxPlay != maxPlay))
				maxPlay = tmp_maxPlay;
				bgfadesw = 0;
			tomoyo_domirror[player] = 0;
			pinch[player] = 0;
			stat[player] = 0;		// それ以外
		}

		for(i=0; i<10; i++) statc[player * 10 + i] = 0;

		if( (stat[1 - player] == 0) || (stat[1 - player] == 10) ) bgfadesw = 1;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.22 - 一定時間何もしない
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：待ち時間
// statc[player * 10 + 1]：待ち時間終了後に移動するステータス
// statc[player * 10 + 3]：ミッションのerace値によるせり上がり
// statc[player * 10 + 4]：↑使用時のせり上がりタイプ
void statNothing(int player) {
	int i;

	if(repversw >= 30) padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	statc[player * 10 + 2]++;

	if(statc[player * 10 + 2] >= statc[player * 10 + 0]) {
		if((statc[player * 10 + 3] != 0) && (gameMode[player] == 8)){
			statc[player * 10 + 3]--;
			upLines[player]++;	//UpLineBlockで-1される
			UpLineBlockJudge(player);
			if(statc[player * 10 + 3] == 0){
				if(mission_type[c_mission] == 21)
					upLineT[player] = 2;
				else
					upLineT[player] = uplinetype;
				pinchCheck(player);
			}
			return;
		}
		if(repversw >= 48){
			if((fmirror_cnt[player] == -20) && (gameMode[player] != 8) && (gameMode[player] >= 4))
				if(UpLineShirase(player)) return;
			if(fldMirrorProc(player)) return;
		}else
			if(fldMirrorProc(player)) return;

		stat[player] = statc[player * 10 + 1];
		for(i=0; i<10; i++) statc[player * 10 + i] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.25 - ライン強制消去
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
// statc[player * 10 + 2]：終了後にステージ読み込み(0=なし 1=あり）
void statDelField(int player) {
	int i, j, y,k,l,waitA;

	statc[player * 10]++;

	if((repversw < 64) || (statc[player * 10 + 2]))
		waitA = 1;
	else
		waitA = 9 - 4 * ((wait2[player] <= 12) && ((timeOn[player]) || (ending[player] == 2) || (repversw >= 65)) );

	if(repversw >= 30) padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if(statc[player * 10] < 15+(wait1[player] * 2))
	for(i = 0; i <= fldsizeh[player]; i++) {
		if(erase[i + player * 22]) {
			if(statc[player * 10] == wait1[player] + 1) {
				if(i >= 2) {
					objectCreate(player, 13, (15 + 24 * player - 12 * maxPlay) * 8, (i + 3) * 8, 0, 0, 0, ((!statc[player * 10 + 2]) && (repversw >= 64)));
				}
			}
			if(statc[player * 10] == wait1[player] + waitA) {
				for(j = 0; j < fldsizew[player]; j++){
					fld[j + i * fldsizew[player] + player * 220] = 0;
					fldt[j + i * fldsizew[player] + player * 220] = 0;
					fldi[j + i * fldsizew[player] + player * 220] = 0;
					grayoutLackedBlock(player, flds[j + i * 10 + player * 220]);
					flds[j + i * fldsizew[player] + player * 220] = 0;
				}
			}
		}
	}

	if(statc[player * 10] == wait1[player] + 1) {
		if(statc[player * 10 + 2]) {
			loadTargetStage();
			dhold[player] = disable_hold;
		}
	}

	if(statc[player * 10] == 15+(wait1[player] * 2)) {
		if((isDWdelfield[player]) || (isdeleven[player])){	//消去後にブロックが落下する場合、落下処理を行う
			y = fldsizeh[player];
			for(i = 0; i <= fldsizeh[player]; i++) {
				if(erase[y + player * 22]) {
					for(k = y; k > 0; k--) {
						for(l = 0; l < fldsizew[player]; l++) {
							fld[l + k * fldsizew[player] + player * 220] = fld[l + (k - 1) * fldsizew[player] + player * 220];
							// #1.60c7j5
							fldt[l + k * fldsizew[player] + player * 220] = fldt[l + (k - 1) * fldsizew[player] + player * 220];
							// #1.60c7n1
							if((gameMode[player] == 4) || (item_mode[player]))
							fldi[l + k * fldsizew[player] + player * 220] = fldi[l + (k - 1) * fldsizew[player] + player * 220];
						}
						erase[k + player * 22] = erase[(k - 1) + player * 22];
					}
					for(l = 0; l < fldsizew[player]; l++) {
						fld[l + player * 220] = 0;
						fldt[l + player * 220] = 0;		// #1.60c7j5
						if((gameMode[player] == 4) || (item_mode[player])) fldi[l + player * 220] = 0;		// #1.60c7n1
						erase[player * 22] = 0;
					}
				} else {
					y--;
				}
			}
			PlaySE(2);
//			for(i = 0; i < 21; i++) erase[i + player * 21] = 0;
		}
	}
	if( statc[player * 10] > 15 + (wait1[player] * 2)+(wait1[player]*( (isDWdelfield[player]) || (isdeleven[player]) )) ) {
		stat[player] = statc[player * 10 + 1];
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 0;
		if(repversw >= 48){
			statc[player * 10 + 1] = stat[player];
			stat[player] = 22;
		}
		pinchCheck(player);
		pinchCheck2(player);
		isUPdelfield[player] = 0;
		isDWdelfield[player] = 0;
		isdeleven[player] = 0;
		if(statc[player * 10 + 2]) {
			dhold[player] = disable_hold;
		}
		statc[player * 10 + 2] = 0;
		for(i = 0; i < 22; i++) erase[i + player * 22] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.26 - レーザー（16t）
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
// statc[player * 10 + 2]：16tか？
void statLaser(int player) {
	int i,j,move,waitA,waitB;

//	dolaser[player] = 0;
	if(repversw < 63){
		waitA = 120;
		waitB = 180;
	}else{
		waitA = 30 + (wait1[player] * 3);
		waitB = waitA + 34 + wait1[player];
	}

	padRepeat(player);
	if((ace_irs) && (statc[player * 10 + 0] > 120)) doIRS2(player);	// ACE式IRS C7U1.5

	if(statc[player * 10 + 0] == 0){	//最初
		if(statc[player * 10 + 2] == 0){//レーザー
			lasernum[player] = 1;
			laserpos[0 + 4 * player] = bx[player] + rand(4,player) - 2;
			if(laserpos[0 + 4 * player] < 0) laserpos[0 + 4 * player] = 0;
			if(laserpos[0 + 4 * player] > fldsizew[player] - 1) laserpos[0 + 4 * player] = fldsizew[player] - 1;
			rapid_c[player] = 0;
			statc[player * 10 + 0]++;
		}else{//16t
			lasernum[player] = 1;
			laserpos[0 + 4 * player] = bx[player] + rand(4,player) - 2;//左端
			if(laserpos[0 + 4 * player] < 0) laserpos[0 + 4 * player] = 0;
			if(laserpos[0 + 4 * player] > fldsizew[player] - 3) laserpos[0 + 4 * player] = fldsizew[player] - 3;
			rapid_c[player] = 0;
			statc[player * 10 + 0]++;
		}
	} else {
		if(statc[player * 10 + 2] == 0){//レーザー
			if(statc[player * 10 + 0] < waitA){	//レーザー発射前
				if(statc[player * 10 + 0] % 5 == 0)
					PlaySE(3);
				if( ( getPushState(player, 4)) || ( getPushState(player, 5)) || ( getPushState(player, 6)) )
					rapid_c[player]++;
				if((rapid_c[player] > 4) && (lasernum[player] < 4)){	//レーザーを増やす（最大4本）
					lasernum[player]++;
					laserpos[lasernum[player] - 1 + 4 * player ] = rand(fldsizew[player] - 1,player);
					rapid_c[player] = 0;
				}
				//照準を移動
				// →	照準は動かしにくくする
				if((getPushState(player, 3)) && (statc[player * 10 + 0] % 10 < 2)){
					for(i = 0; i < lasernum[player]; i++){
						if(laserpos[i + 4 * player] < fldsizew[player] - 1) laserpos[i + 4 * player]++;
					}
				}
				// ←	移動入力の受付は一定間隔で
				if((getPushState(player, 2)) && (statc[player * 10 + 0] % 10 < 2)){
					for(i = 0; i < lasernum[player]; i++){
						if(laserpos[i + 4 * player] > 0) laserpos[i + 4 * player]--;
					}
				}
				// 照準を描画
				for(i = 0; i < lasernum[player]; i++){
					ExBltRect(78, ((laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) - 1) * 8, (4 - 1) * 8, 0, ((statc[player * 10 + 0] % 36) / 6) * 24, 24, 24);
				}
			} else if(statc[player * 10 + 0] == waitA){	// レーザー発射
				PlaySE(12);
				for(i = 0; i < lasernum[player]; i++){	// 照準直下のブロックを消去
					ExBltRect(78, (laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) * 8, 5 * 8, 24, 0, 8, 160);
					ExBltRect(78, ((laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) - 1) * 8, (4 - 1) * 8, 0, 0, 24, 24);
					for(j = 0; j <= fldsizeh[player]; j++){
						if(fld[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] != 0){
							objectCreate(player, 1, (laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) * 8, (j + 3) * 8, (laserpos[i + 4 * player] - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150), fld[laserpos[i + 4 * player] + j * 10 + player * 220], 100);
							fld[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;
							fldt[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;
							fldi[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;
							grayoutLackedBlock(player, flds[laserpos[i + player * 4] + j * 10 + player * 220]);
							flds[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;

						}
					}
				}
				scanItem(player);
			} else {	// レーザー発射後
				for(i = 0; i < lasernum[player]; i++){
					if(statc[player * 10 + 0] - waitA < 34){
						j = (statc[player * 10 + 0] - waitA) / 2;
						ExBltRect(78, (laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) * 8, 5 * 8, 24+(j*8), 0, 8, 160);
					}
					ExBltRect(78, ((laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) - 1) * 8, (4 - 1) * 8, 0, 0, 24, 24);
				}
			}
		}else{//16t
			if(statc[player * 10 + 0] < waitA){	//レーザー発射前
				if(statc[player * 10 + 0] % 6 == 0)
					PlaySE(5);//カチカチ
				laserpos[1 + 4 * player ] = laserpos[0 + 4 * player ] + 1;//左から二番目
				laserpos[2 + 4 * player ] = laserpos[0 + 4 * player ] + 2;//左から3番目
				//照準を移動
				// →	照準は動かしにくくする
				if(getPushState(player, 3)){
						if(laserpos[0 + 4 * player] < fldsizew[player] - 3) laserpos[0 + 4 * player]++;
				}
				// ←	移動入力の受付は一定間隔で
				if(getPushState(player, 2)){
						if(laserpos[0 + 4 * player] > 0) laserpos[0 + 4 * player]--;
				}
				// 錘を描画
				ExBltRect(3, ((laserpos[0 + 4 * player] + 15 + 24 * player - 12 * maxPlay) - 1) * 8, 10, 260, 399, 40, 21);
				printFont(16+laserpos[0 + 4 * player]+24 * player - 12 * maxPlay, 5, "k", (count % 2));
				printFont(15+24 * player - 12 * maxPlay, 6, "<L R>:MOVE", (count % 2));
			} else if(statc[player * 10 + 0] >= waitA){	// レーザー発射
				for(i = 0; i < 3; i++){	// 照準直下のブロックを消去
//					for(j = 0; j <= fldsizeh[player]; j++){
					j = (statc[player * 10 + 0] - waitA) * 2;
					if((j - 1 >= 0) && (j - 1 <= fldsizeh[player])){
						if(fld[laserpos[i + 4 * player] + (j-1) * fldsizew[player] + player * 220] != 0){
						//	objectCreate(player, 1, (laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) * 8, (j + 2) * 8, (laserpos[i + 4 * player] - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150), fld[laserpos[i + 4 * player] + j * 10 + player * 220], 100);
							fld[laserpos[i + 4 * player] + (j-1) * fldsizew[player] + player * 220] = 0;
							fldt[laserpos[i + 4 * player] + (j-1) * fldsizew[player] + player * 220] = 0;
							fldi[laserpos[i + 4 * player] + (j-1) * fldsizew[player] + player * 220] = 0;
							grayoutLackedBlock(player, flds[laserpos[i + player * 4] + (j-1) * 10 + player * 220]);
							flds[laserpos[i + 4 * player] + (j-1) * fldsizew[player] + player * 220] = 0;
							PlaySE(2);
						}
					}
					if((j >= 0) && (j <= fldsizeh[player])){
						if(fld[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] != 0){
						//	objectCreate(player, 1, (laserpos[i + 4 * player] + 15 + 24 * player - 12 * maxPlay) * 8, (j + 3) * 8, (laserpos[i + 4 * player] - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150), fld[laserpos[i + 4 * player] + j * 10 + player * 220], 100);
							fld[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;
							fldt[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;
							fldi[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;
							grayoutLackedBlock(player, flds[laserpos[i + player * 4] + j * 10 + player * 220]);
							flds[laserpos[i + 4 * player] + j * fldsizew[player] + player * 220] = 0;
							PlaySE(2);
						}
					}
				}
				if(j <= fldsizeh[player]) scanItem(player);
			//	j = statc[player * 10 + 0] - waitA;
				ExBltRect(3, ((laserpos[0 + 4 * player] + 15 + 24 * player - 12 * maxPlay) - 1) * 8, ((j + 3)*8) - 20, 260, 399, 40, 21);
			}
		}

		statc[player * 10 + 0]++;

		if(statc[player * 10 + 0] >= waitB){
			stat[player] = statc[player * 10 + 1];
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			statc[player * 10 + 2] = 0;
			if(repversw >= 48){
				statc[player * 10 + 1] = stat[player];
				stat[player] = 22;
			}
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.27 - ネガフィールド
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
void statNegafield(int player) {
	int i,j,y,waitA;

//	donega[player] = 0;
	if(repversw < 63)
		waitA = 120;
	else
		waitA = 90 + wait1[player];

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if(statc[player * 10 + 0] == 0){	//最初
		for(i = 0;i < 10; i++)
//			nega_pos[i + 10 * player] = checkFieldTop2(player,i);
			nega_pos[i + 10 * player] = checkFieldTop(player);
		statc[player * 10 + 0]++;
	} else {
		if(statc[player * 10 + 0] <= 88){
		if(statc[player * 10 + 0] % 4 == 0){
			y = 22 - (statc[player * 10 + 0] / 4);
			for(i = 0;i < 10; i++){
				if(fld[i + y * fldsizew[player] + player * 220] != 0){	// ブロックを空白に
					PlaySE(5);
					fld[i + y * fldsizew[player] + player * 220] = 0;
					fldt[i + y * fldsizew[player] + player * 220] = 0;
					fldi[i + y * fldsizew[player] + player * 220] = 0;
					grayoutLackedBlock(player, flds[i + y * 10 + player * 220]);
					flds[i + y * fldsizew[player] + player * 220] = 0;
				} else if(y >= nega_pos[i + 10 * player]){			// 空白をブロックに
					PlaySE(5);
					fld[i + y * fldsizew[player] + player * 220] = (y % 7) + 2;
					fldt[i + y * fldsizew[player] + player * 220] = -1;
					grayoutLackedBlock(player, flds[i + y * 10 + player * 220]);
					flds[i + y * fldsizew[player] + player * 220] = 0;
					if((hidden[player] == 8) && (m_roll_blockframe))
						fldt[i + y * fldsizew[player] + player * 220] = -60;
					else if(hidden[player] == 8)
						fldt[i + y * fldsizew[player] + player * 220] = 0;
				}
			}
			scanItem(player);
		}
		}
		statc[player * 10 + 0]++;
		if(statc[player * 10 + 0] >= waitA){
			stat[player] = statc[player * 10 + 1];
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			if(repversw >= 48){
				statc[player * 10 + 1] = stat[player];
				stat[player] = 22;
			}
		}
	}

}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.28 - ショットガン
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
void statShotgun(int player) {
	int i,j,x,limit,waitA,waitB;
	j = 0;

//	doshotgun[player] = 0;
	limit = 0;

	if(repversw < 63){
		waitA = 60;
		waitB = 120;
	}else{
		waitA = 6 + (wait1[player] * 2);
		waitB = waitA + 30 + wait1[player];
	}

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if(statc[player * 10 + 0] == 0){	//最初
		for(i = checkFieldTop(player); i < 22; i++){
			if(i == 0){
				do{
					x = rand(fldsizew[player],player);
				} while(fld[x + i * fldsizew[player] + player * 220] == 0);
			} else {
				do{
					x = rand(fldsizew[player],player);
					limit++;
					if(limit > 100000) break;
				} while( ((x == shotgunpos[(i - 1) + 22 * player]) && (limit < 50000)) || (fld[x + i * fldsizew[player] + player * 220] == 0) );
			}

			shotgunpos[i + 22 * player] = x;
		}
		statc[player * 10 + 0]++;
	} else {
		if(statc[player * 10 + 0] == waitA){
			PlaySE(14);
			thunder_timer = 10;
			for(i = checkFieldTop(player); i < 22; i++){
				if(fld[shotgunpos[i + 22 * player] + i * fldsizew[player] + player * 220] != 0){
					objectCreate(player, 1, (shotgunpos[i + 22 * player] + 15 + 24 * player - 12 * maxPlay) * 8, (i + 3) * 8, (shotgunpos[i + 22 * player] - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150), fld[shotgunpos[i + 22 * player] + i * 10 + player * 220], 100);
					fld[shotgunpos[i + 22 * player] + i * fldsizew[player] + player * 220] = 0;
					fldt[shotgunpos[i + 22 * player] + i * fldsizew[player] + player * 220] = 0;
					fldi[shotgunpos[i + 22 * player] + i * fldsizew[player] + player * 220] = 0;
					grayoutLackedBlock(player, flds[j + i * 10 + player * 220]);
					flds[shotgunpos[i + 22 * player] + i * fldsizew[player] + player * 220] = 0;
				}
			}
			scanItem(player);
		}
		statc[player * 10 + 0]++;
		if(statc[player * 10 + 0] >= waitB){
			stat[player] = statc[player * 10 + 1];
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			if(repversw >= 48){
				statc[player * 10 + 1] = stat[player];
				stat[player] = 22;
			}
		}
	}

}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.29 - フィールド交換
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
// statc[player * 10 + 2]：相手と同期を取るためのフラグ
// statc[player * 10 + 3]：自分は交換しない（フィールドコピー）
void statExchangefield(int player) {
	int i,j,enemy;

	doexchg[player] = 0;
	docopyfld[player] = 0;
	enemy = 1 - player;		//対象を決める
	if(gameMode[player] != 4)
		enemy = player;

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if(statc[player * 10 + 0] == 0){	//最初
		if( ((stat[enemy] == 29) && (statc[enemy * 10 + 2] == 1)) || (item_mode[player]) ){	//同期チェック
			// フィールドをバッファに確保（変更前）
			if(repversw < 48){
				for(i = 0; i <= fldsizeh[player]; i++) {
					for(j = 0; j < fldsizew[player]; j++) {
						fldbuf[j + i * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
						fldtbuf[j + i * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
						fldibuf[j + i * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
						fldsbuf[j + i * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
					}
				}
			}
			statc[player * 10 + 0]++;
		}
		statc[player * 10 + 2] = 1;
	} else {
		// 相手フィールドを自分のバッファに確保（交換開始直前に変更）
		if((statc[player * 10 + 0] == 58) && (repversw >= 48)){
			for(i = 0; i < 220; i++) {
				fldbuf[i + player * 220] = fld[i + enemy * 220];
				fldtbuf[i + player * 220] = fldt[i + enemy * 220];
				fldibuf[i + player * 220] = fldi[i + enemy * 220];
				fldsbuf[i + player * 220] = flds[i + enemy * 220];
			}
		}

		if((statc[player * 10 + 0] >= 60) && (statc[player * 10 + 0] < 70) && (!statc[player * 10 + 3])){
			for(i = 0; i < 22; i++) {
				j = statc[player * 10 + 0] - 60;
				fld[j + i * 10 + player * 220] = fldbuf[j + i * 10 + player * 220];
				fldt[j + i * 10 + player * 220] = fldtbuf[j + i * 10 + player * 220];
				fldi[j + i * 10 + player * 220] = fldibuf[j + i * 10 + player * 220];
				flds[j + i * 10 + player * 220] = fldsbuf[j + i * 10 + player * 220];
				if((fld[j + i * 10 + player * 220] < 0) && (rots[player] != 6))
					fld[j + i * 10 + player * 220] = 8;
			}
		ExBltFast(27, 120+ ((statc[player * 10 + 0] - 60) * 8) + 192 * player - 96 * maxPlay, 40);
		scanItem(player);
		}
		if(gameMode[player] != 4)		// 1人用アイテムモードでは効果なし
			printFont(15 + 24 * player - 12 * maxPlay, 15, "NO EFFECT", fontc[rots[player]]);
		statc[player * 10 + 0]++;
		if(statc[player * 10 + 0] >= 120){
			stat[player] = statc[player * 10 + 1];
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 3] = 0;
			if(repversw >= 48){
				statc[player * 10 + 1] = stat[player];
				stat[player] = 22;
			}
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.30 - 段位認定試験っぽいの
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：0なら開始宣言、他なら結果発表

void statExamination(int player){
	int i,k;
	if(statc[player * 10 + 1] == 0){		//開始宣言
		if(statc[player * 10 + 0] == 0){//プルプル
			PlaySE(7);
			purupuru[player] = 1;
		} else if((statc[player * 10 + 0] >= 0) && (statc[player * 10 + 0] < 110)){
			ofs_x[player] = Rand(16) - 8;
			ofs_x2[player] = ofs_x[player];
		} else if(statc[player * 10 + 0] == 110){//プルプルおわり
			PlaySE(39);
			purupuru[player] = 0;
			ofs_x[player] = 0;
			ofs_x2[player] = 0;
		} else if(statc[player * 10 + 0] > 110){		//試験ですよ～
			if(statc[player * 10 + 0] < 170)
				ExBltRectR(81, 110 - ((100 * 22 / 12 - 100) / 2) +192 * player -96 * maxPlay , 80 , 200, 100 + 25 * (count % 4 / 2), 100, 25,65536 * 22 / 12,65536 * 22 / 12);
			else if(statc[player * 10 + 0] < 192){
				k = 192 - statc[player * 10 + 0];
				i = (100 * k / 12 - 100) / 2;
				k = 65536 * k / 12;
				if(k < 65536){
					k = 65536;
					i=0;
				}
				ExBltRectR(81, 110 - i + 192 * player -96 * maxPlay , 48 + 192 - statc[player * 10 + 0] , 200, 100 + 25 * (count % 4 / 2), 100, 25,k,k);
			}else
				ExBltRect(81, 110+192 * player -96 * maxPlay , 48 , 200, 100 + 25 * (count % 4 / 2), 100, 25);
			if(statc[player * 10 + 0] > 210){		//目標段位の表示
				ExBltRect(81, 110+192 * player -96 * maxPlay , 74 , 200, 180, 100, 20);
				printSMALLFont(122 + 192 * player - 96 * maxPlay, 93, " Examination",0);
				printSMALLFont(122 + 192 * player - 96 * maxPlay, 102, "    Grade     ",0);
				if(exam_grade[player]== 32) {
					// GM
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 115, 270 ,((count % 4 / 2)*216)+120,90,48);
				} else if( exam_grade[player] >= 28 ) {
					// MK-MM
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 270 ,((count % 4 / 2)*216)+((exam_grade[player]-28)*24),90,24);
				} else if( exam_grade[player] >= 27 ) {
					if((enable_grade[player]==2)||(enable_grade[player]==4)){
						// M
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 127,270 ,((count % 4 / 2)*216)+((exam_grade[player]-27)*24),90,24);
					}else{
						// m
						ExBltRect(26,124 + 192 * player -96 * maxPlay , 127,450 ,((count % 4 / 2)*216)+((exam_grade[player]-27)*24),90,24);
					}
				} else if( exam_grade[player] >= 18 ) {
					// m1～m9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 180 ,((count % 4 / 2)*216)+((exam_grade[player]-18)*24),90,24);
				} else if( exam_grade[player] >= 9 ) {
					// S1～S9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 90 ,((count % 4 / 2)*216)+((exam_grade[player]-9)*24),90,24);
				} else {
					// 9～1
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 0 ,((count % 4 / 2)*216)+(exam_grade[player]*24),90,24);
				}
			}
			if(statc[player * 10 + 0] > 360){
				// ゲーム開始
				stat[player] = 3;				// Ready
				statc[player * 10 + 0] = 0;		// ステータスカウンタを0に
				statc[player * 10 + 1] = 0;
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;
				return;
			}
		}
	} else {		// 結果発表.
		if(statc[player * 10 + 0] == 0){
			purupuru[player] = 2;
			endingcnt[player] = 0;
		} else if((statc[player * 10 + 0] >= 0) && (statc[player * 10 + 0] < 60)){
			ofs_x[player] = Rand(8) - 4;
			ofs_x2[player] = ofs_x[player];
		} else if(statc[player * 10 + 0] == 60){
			PlaySE(39);
			purupuru[player] = 0;
			ofs_x[player] = 0;
			ofs_x2[player] = 0;
		} else if(statc[player * 10 + 0] > 60){		//結果発表～
			ExBltRect(81, 110+192 * player -96 * maxPlay , 48 , 200, 150, 100, 30);
			printSMALLFont(119 + 192 * player - 96 * maxPlay, 78, "    RESULT    ",0);
			if(statc[player * 10 + 0] > 90){		//目標段位の表示
				printSMALLFont(122 + 192 * player - 96 * maxPlay, 93, " Examination",0);
				printSMALLFont(122 + 192 * player - 96 * maxPlay, 102, "    Grade     ",0);
				if(exam_grade[player] == 32) {
					// GM
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 115, 270 ,((count % 4 / 2)*216)+120,90,48);
				} else if( exam_grade[player] >= 28 ) {
					// M-MM
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 270 ,((count % 4 / 2)*216)+((exam_grade[player]-28)*24),90,24);
				} else if( exam_grade[player] >= 27 ) {
					if((enable_grade[player]==2)||(enable_grade[player]==4)){
						// M
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 127,270 ,((count % 4 / 2)*216)+((exam_grade[player]-27)*24),90,24);
					}else{
						// m
						ExBltRect(26,115 + 192 * player -96 * maxPlay , 127,450 ,((count % 4 / 2)*216)+((exam_grade[player]-27)*24),90,24);
					}
				}else if( exam_grade[player] >= 18 ) {
					// m1～m9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 180 ,((count % 4 / 2)*216)+((exam_grade[player]-18)*24),90,24);
				} else if( exam_grade[player] >= 9 ) {
					// S1～S9
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 90 ,((count % 4 / 2)*216)+((exam_grade[player]-9)*24),90,24);
				} else {
					// 9～1
					ExBltRect(26,115 + 192 * player -96 * maxPlay , 127, 0 ,((count % 4 / 2)*216)+(exam_grade[player]*24),90,24);
				}
			}
			if(statc[player * 10 + 0] == 150){
				if((abs_YGS2K(exam_grade[player] - grade[player]) > 3) || (grade[player] == 32))	//差が大きければルーレット演出をとばす
					statc[player * 10 + 0] = 350;
			}
			if((statc[player * 10 + 0] > 150) && (statc[player * 10 + 0] < 350)){	//ルーレット
				if((statc[player * 10 + 0] % 5 == 0) && (statc[player * 10 + 0] < 320))
					PlaySE(5);
				if((statc[player * 10 + 0] % 10 < 5) || (statc[player * 10 + 0] >= 320)){
					ExBltRect(81, 125+192 * player -96 * maxPlay , 162 , 215, 250, 70, 25);
					printSMALLFont(125 + 192 * player - 96 * maxPlay, 189, "   FAILED   ",0);
				}else{
					ExBltRect(81, 125+192 * player -96 * maxPlay , 162 , 215, 200, 70, 25);
					printSMALLFont(122 + 192 * player - 96 * maxPlay, 189, "   SUCCESS   ",0);
				}
			}
			if(statc[player * 10 + 0] == 350){
				if(exam_grade[player] <= grade[player]){	//合格
					PlaySE(18);
					if((examination[player]==1) && (enable_grade[player] == 4) && (!playback)){
						admit_grade[player]=exam_grade[player];
					}
				}else{									//不合格
					PlaySE(45);
					if((examination[player]==2) && (enable_grade[player] == 4) && (!playback)){
						admit_grade[player]=exam_grade[player];
					}
				}
			}
			if(statc[player * 10 + 0] >= 350){
				if(exam_grade[player] <= grade[player]){	//合格
					if((statc[player * 10] % 10 == 0)&&(endingcnt[player] < 20)) {
						PlaySE(35);
						objectCreate2(player, 7, Rand(80) + 72 + 192 * player - 96 * maxPlay, 32 + Rand(20), 0, 0, Rand(7)+1, 0);
						endingcnt[player]++;
					}
					ExBltRect(81, 125+192 * player -96 * maxPlay , 162 , 215, 200 + 25 * (count % 4 / 2), 70, 25);
					printSMALLFont(122 + 192 * player - 96 * maxPlay, 189, "   SUCCESS   ",(count % 4 / 2) * digitc[rots[player]]);
				}else{										//不合格
					ExBltRect(81, 125+192 * player -96 * maxPlay , 162 , 215, 250 + 25 * (count % 4 / 2), 70, 25);
					printSMALLFont(125 + 192 * player - 96 * maxPlay, 189, "   FAILED   ",(count % 4 / 2) * 2);
				}
			}

			if(statc[player * 10 + 0] > 720){
				if(exam_ranking){stat[player] = 11;//ネームエントリー
				}else {
					PlayerdataSave();
					stat[player]=14;

				}	// 結果
				grade_pasttime[player] = 0;		//前の試験からの経過時間をリセット
				statc[player * 10 + 0] = 0;		// ステータスカウンタを0に
				statc[player * 10 + 1] = 0;
				statc[player * 10 + 2] = 0;
				statc[player * 10 + 3] = 0;
				return;
			}
		}
	}
	statc[player * 10 + 0]++;
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.31 - アイテムルーレット
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
// statc[player * 10 + 2]：発動アイテム
void statItemRulet(int player) {
	int i,j,y,waitA;

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if(repversw < 63){
		waitA = 180;
	}else{
		waitA = 150 + wait1[player];
	}

	if(statc[player * 10 + 0] == 0){	//最初

		statc[player * 10 + 0]++;
	} else {
		if(statc[player * 10 + 0] <= 119){//シャッフル中
			if(statc[player * 10 + 0] % 3 == 0){
				PlaySE(5);
				statc[player * 10 + 2] = Rand(item_num) + 1;
			}
		}
		if(statc[player * 10 + 0] == 120){//決定
			PlaySE(10);
			statc[player * 10 + 2] = rand(item_num,player) + 1;
			if(repversw < 48)
				eraseItem(player, statc[player * 10 + 2]);
			else if((rand(10,player) < 1) || (statc[player * 10 + 2] == item_num + 1)){	//スカ
				statc[player * 10 + 2] = 36;
				PlaySE(45);
				if(repversw >= 54){
					eraseItem(player, statc[player * 10 + 2]);
					item_t[player] = item_interval / 4;
				}
			}else{//何かか決定
				eraseItem(player, statc[player * 10 + 2]);
				if((repversw >= 54) && (statc[player * 10 + 2] == 36)){
					PlaySE(45);
					misstimer[player] = item_interval / 4;
				}
			}

			if(stat[player] != 31){
				statc[player * 10 + 1] = stat[player];
				stat[player] = 31;
			}
			if((dorulet[player]) && (repversw >= 48) && (repversw < 62)){
				dorulet[player] = 0;
				eraseItem(player, rand(item_num,player) + 1);
			}
		}
		if((statc[player * 10 + 0] > 120) && (statc[player * 10 + 0] <= 150) && (statc[player * 10 + 2] == 36)){
			y = Rand(8);
			ofs_x[player] = y - (8 / 2);
			ofs_x2[player] = ofs_x[player];
		}else{
			ofs_x[player] = 0;
			ofs_x2[player] = 0;
		}
		if(statc[player * 10 + 0] <= 150)ExBltFastRect(89, 120+192 * player -96 * maxPlay , 142 , 80 * ((statc[player * 10 + 2] - 1) / 10), 36 * ((statc[player * 10 + 2] - 1) % 10), 80, 36);
		ExBltFastRect(46,(4 + 24 * player + 12 * (!maxPlay))*8, 182,(8*(statc[player * 10 + 2] - 1))+56,0,8,8);
		ExBltFastRect(46,(11 + 24 * player + 12 * (!maxPlay))*8, 182,(8*(statc[player * 10 + 2] - 1))+56,0,8,8);
		ExBltRect(28,(5 + 24 * player + 12 * (!maxPlay))*8, 178, 0, (statc[player * 10 + 2] - 1) * 16, 48, 16);
		statc[player * 10 + 0]++;
		if(statc[player * 10 + 0] >= waitA){
			if(repversw >= 48){
				if(fmirror_cnt[player] == -20)
					if(UpLineShirase(player)) return;
				if(fldMirrorProc(player)) return;
			}else if(repversw >= 40)
				if(fldMirrorProc(player)) return;
			stat[player] = statc[player * 10 + 1];
			statc[player * 10] = 0;
			statc[player * 10 + 1] = 0;
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 3] = 0;
			statc[player * 10 + 4] = 0;
	// アイテム発動の対象が自分だった場合
	//↑DEL FIELD
	if(isUPdelfield[player]){
		j = (22-checkFieldTop(player)) / 2;
		for(i = checkFieldTop(player); i <= 22; i++) {
			erase[i + player * 22] = 1;
			j--;
			if(j < 0) break;
		}
		stat[player] = 25;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	//↓DEL FIELD
	if(isDWdelfield[player]){
		j = (22-checkFieldTop(player)) / 2;
		for(i = 22; i > checkFieldTop(player); i--) {
			erase[i + player * 22] = 1;
			j--;
			if(j < 0) break;
		}
		stat[player] = 25;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	//DEL EVEN
	if(isdeleven[player]){
		for(i = 22; i >= checkFieldTop(player); i--) {
			erase[i + player * 22] = 1;
			i--;
		}
		stat[player] = 25;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	//FREE FALL
	if(isFreefall[player]){
		stat[player] = 32;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		return;
	}
	//MOV FIELD
	if((isLmovfield[player]) || (isRmovfield[player])){
		stat[player] = 33;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		return;
	}
		//ALLCLEAR
	if(isallclear[player]){
//		for(i = 0; i < 22; i++) {
//			erase[i + player * 22] = 1;
//		}
		stat[player] = 35;
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 4;	// 落下開始
		statc[player * 10] = -wait1[player];
		statc[player * 10 + 2] = 21;
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		return;
	}
	if(doshotgun[player]){	//ショットガン発動
		stat[player] = 28;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		doshotgun[player] = 0;
		return;
	}
	if(dolaser[player]){	//レーザー発動
		stat[player] = 26;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		dolaser[player] = 0;
		return;
	}
	if(donega[player]){	//ネガフィールド発動
		stat[player] = 27;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		donega[player] = 0;
		return;
	}
	if(dorulet[player]){	//アイテムルーレット発動
		stat[player] = 31;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		dorulet[player] = 0;
		return;
	}
	if(do180field[player]){	//180°フィールド発動
		stat[player] = 34;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		do180field[player] = 0;
		return;
	}
	if(doexchg[player]){	//フィールド交換発動
		stat[player] = 29;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 0;
		statc[player * 10 + 4] = 0;
		doexchg[player] = 0;
		return;
	}
	if(docopyfld[player]){	//フィールドコピー発動
		stat[player] = 29;
		statc[player * 10 + 0] = 0;
		statc[player * 10 + 1] = 4;		// ステータス
		if(ending[player])				//エンディング時
			statc[player * 10 + 1] = 13;
		statc[player * 10 + 2] = 0;
		statc[player * 10 + 3] = 1;
		statc[player * 10 + 4] = 0;
		docopyfld[player] = 0;
		return;
	}
		}
	}

}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.32 - フリーフォール
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
// statc[player * 10 + 2]：再せり上がり時の穴の位置
void statFreefall(int player) {
	int i,j,x,y = 0,l,k,tmp;

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if((statc[player * 10 + 0] >= wait1[player]) && (statc[player * 10 + 0] <= (wait1[player] * 2) + 40)){
		if(statc[player * 10 + 0] < wait1[player] + 25){
			ofs_y[player] = ofs_y[player] - 2;
			ofs_y2[player] = ofs_y2[player] - 2;
		}
		if((statc[player * 10 + 0] >= wait1[player] + 25) && (statc[player * 10 + 0] < wait1[player] + 30)){
			ofs_y[player] = ofs_y[player] + 10;
			ofs_y2[player] = ofs_y2[player] + 10;
		}
		if(statc[player * 10 + 0] == wait1[player] + 30){	//自由落下処理
			ofs_y[player] = 0;
			ofs_y2[player] = 0;
			PlaySE(2);
			for(y = fldsizeh[player] - 1;y >= 0;y--){	//最下段は調べない
				for(i = 0;i < fldsizew[player];i++){
					if((fld[i + y * fldsizew[player] + player * 220] != 0) && (fld[i + (y + 1) * fldsizew[player] + player * 220] == 0)){	//ブロックかつ1段下が空白なら
						tmp = y;
						do{
							if(tmp + 1 > fldsizeh[player]) break;	//最下段に到達
							j = fld[i + tmp * fldsizew[player] + player * 220];
							fld[i + tmp * fldsizew[player] + player * 220] = 0;
							fld[i + (tmp + 1) * fldsizew[player] + player * 220] = j;
							j = fldt[i + tmp * fldsizew[player] + player * 220];
							fldt[i + tmp * fldsizew[player] + player * 220] = 0;
							fldt[i + (tmp + 1) * fldsizew[player] + player * 220] = j;
							j = fldi[i + tmp * fldsizew[player] + player * 220];
							fldi[i + tmp * fldsizew[player] + player * 220] = 0;
							fldi[i + (tmp + 1) * fldsizew[player] + player * 220] = j;
							j = flds[i + tmp * fldsizew[player] + player * 220];
							flds[i + tmp * fldsizew[player] + player * 220] = 0;
							flds[i + (tmp + 1) * fldsizew[player] + player * 220] = j;
							tmp++;
						}while(fld[i + (tmp + 1) * fldsizew[player] + player * 220] == 0);
					}
				}
			}
		}
	}
	if(statc[player * 10 + 0] == (wait1[player] * 2) + 30){	//消去判定（なければ次のステータスへ）
		if(blockEraseJudgeFf(player,0) == 0){	// 消去なし
			statc[player * 10 + 0] = -100;
		} else {							// 消去あり
			statc[player * 10 + 2] = rand(10,player);
			statc[player * 10 + 3] = statc[player * 10 + 2];
			if(IsBigStart[player]) {
				statc[player * 10 + 2] = (statc[player * 10 + 2] / 2) * 2;
				statc[player * 10 + 3] = statc[player * 10 + 2] + 1;
			}

			for(i = 0; i <= fldsizeh[player]; i++) {
				if(erase[i + player * 22]) {
					Ff_rerise[player]++;
					y--;
					for(x = 0; x < fldsizew[player]; x++) { // #1.60c7b
						if(breakeffect) {
							// super_breakeffectが1の場合は全てのブロックに破壊エフェクトを適用する #1.60c7m9
							// プラチナブロックが消える場合は無条件
							if( (fld[x + i * fldsizew[player] + player * 220] >= 11) || (super_breakeffect == 1) ||
								( ((breaktype == 0)||((breaktype == 3)&&(gameMode[player] == 0))) && (super_breakeffect == 2) ) ||
								((heboGB[player]) && (super_breakeffect == 2)) ) {
								objectCreate(player, 1, (x + 15 + 24 * player - 12 * maxPlay) * 8, (i + 3) * 8, (x - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + Ff_rerise[player] * 250, fld[x + i * 10 + player * 220], 100);
							} else if(Ff_rerise[player] & 1) {
								if((x & 1) == 1) {
									objectCreate(player, 1 + (wait1[player] < 6) * 2, (x + 15 + 24 * player - 12 * maxPlay) * 8 + 4, (i + 3) * 8, (x - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + Ff_rerise[player] * 250, fld[x + i * 10 + player * 220], 100);
								}
							} else {
								if((x & 1) == 0) {
									objectCreate(player, 1 + (wait1[player] < 6) * 2, (x + 15 + 24 * player - 12 * maxPlay) * 8 + 4, (i + 3) * 8, (x - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150) + Ff_rerise[player] * 250, fld[x + i * 10 + player * 220], 100);
								}
							}
						}

						fld[x + i * fldsizew[player] + player * 220] = 0;
						fldt[x + i * fldsizew[player] + player * 220] = 0;	// #1.60c7j5
						fldi[x + i * fldsizew[player] + player * 220] = 0;	// #1.60c7n1
						grayoutLackedBlock(player, flds[x + i * 10 + player * 220]);
						flds[x + i * fldsizew[player] + player * 220] = 0;
					}
				}
			}
			if(gameMode[player] != 4)
				PlaySE(11);
			else if( Ff_rerise[player] >= 5 )
				PlaySE(10 + 4);		// 5ライン以上の場合は4ラインの音を出す
			else
				PlaySE(10 + Ff_rerise[player]);
			if(gameMode[player] == 4){
				li[player] = li[player] + Ff_rerise[player];
				if(Ff_rerise[player] >= 2) blockEraseJudgeFf(player,1);
			}
		}
	}
	if(statc[player * 10 + 0] == (wait1[player] * 2) + 30 + wait2[player]) {	//消去判定があった場合の落下処理
		y = fldsizeh[player];
		for(i = 0; i <= fldsizeh[player]; i++) {
			if(erase[y + player * 22]) {
				for(k = y; k > 0; k--) {
					for(l = 0; l < fldsizew[player]; l++) {
						fld[l + k * fldsizew[player] + player * 220] = fld[l + (k - 1) * fldsizew[player] + player * 220];
						// #1.60c7j5
						fldt[l + k * fldsizew[player] + player * 220] = fldt[l + (k - 1) * fldsizew[player] + player * 220];
						// #1.60c7n1
						if((gameMode[player] == 4) || (item_mode[player]))
						fldi[l + k * fldsizew[player] + player * 220] = fldi[l + (k - 1) * fldsizew[player] + player * 220];
					}
					erase[k + player * 22] = erase[(k - 1) + player * 22];
				}
				for(l = 0; l < fldsizew[player]; l++) {
					fld[l + player * 220] = 0;
					fldt[l + player * 220] = 0;		// #1.60c7j5
					if((gameMode[player] == 4) || (item_mode[player])) fldi[l + player * 220] = 0;		// #1.60c7n1
					erase[player * 22] = 0;
				}
			} else {
				y--;
			}
		}
		PlaySE(2);
		for(i = 0; i <= fldsizeh[player]; i++) erase[i + player * 22] = 0;
	}

	if(statc[player * 10 + 0] == (wait1[player] * 3) + 30 + wait2[player]) statc[player * 10 + 0] = -100;
	//noviceモードとsquareモードだと再せり上がりなし　対戦では相手をせり上げる
	if(((gameMode[player] == 0) && (novice_mode[player]))||(squaremode[player])||(gameMode[player] == 4)) Ff_rerise[player] = 0;

//	if(statc[player * 10 + 0] >= 0)
	statc[player * 10 + 0]++;
	if(statc[player * 10 + 0] < 0){
		if(Ff_rerise[player] != 0){	//再せり上がり
			PlaySE(20);
			for(i = 1; i <= fldsizeh[player]; i++) {
				for(j = 0; j < fldsizew[player]; j++) {
					fld[j + (i - 1) * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
					fldt[j + (i - 1) * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
					flds[j + (i - 1) * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
					if((gameMode[player] == 4) || (item_mode[player]))
						fldi[j + (i - 1) * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
				}
				erase[(i - 1) + player * 22] = erase[i + player * 22];
			}

			for(j = 0; j < fldsizew[player]; j++) {
				fld[j + fldsizew[player] * fldsizeh[player] + player * 220] = ((j != statc[player * 10 + 2]) && (j != statc[player * 10 + 3])) * (rand(7,player) + 2);
				fldt[j + fldsizew[player] * fldsizeh[player] + player * 220] = (fld[j + fldsizew[player] * fldsizeh[player] + player * 220] != 0) * -1;
				// アイテムを増殖させない #1.60c7o6
				if((gameMode[player] == 4) || (item_mode[player])) fldi[j + fldsizew[player] * fldsizeh[player] + player * 220] = 0;
			}
			erase[fldsizeh[player] + player * 22] = 0;
			Ff_rerise[player]--;
			return;
		}else{
			isFreefall[player] = 0;
			stat[player] = statc[player * 10 + 1];
			statc[player * 10 + 1] = 0;
			if((repversw >= 48) && (statc[player * 10 + 0] == -99)){
				statc[player * 10 + 1] = stat[player];
				stat[player] = 22;
			}
			statc[player * 10] = 0;
			statc[player * 10 + 2] = 0;
			statc[player * 10 + 3] = 0;
		}
	}

}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.33 - ブロックを端に寄せる
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
void statMovfield(int player) {
	int i,j,x,y,l,k,tmp;

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if((statc[player * 10 + 0] >= wait1[player]) && (statc[player * 10 + 0] <= (wait1[player] * 2) + 40)){
		if(statc[player * 10 + 0] < wait1[player] + 25){
			ofs_x[player] = ofs_x[player] - 2 + (4 * isLmovfield[player]);
			ofs_x2[player] = ofs_x2[player] - 2 + (4 * isLmovfield[player]);
		}
		if((statc[player * 10 + 0] >= wait1[player] + 25) && (statc[player * 10 + 0] < wait1[player] + 30)){
			ofs_x[player] = ofs_x[player] + 10 - (10 * isLmovfield[player]);
			ofs_x2[player] = ofs_x2[player] + 10 - (10 * isLmovfield[player]);
		}
		if(statc[player * 10 + 0] == wait1[player] + 30){	//処理
			ofs_x[player] = 0;
			ofs_x2[player] = 0;
			PlaySE(2);
			if(isLmovfield[player])	//←MOV FIELD
			for(i = 1;i < fldsizew[player];i++){
				for(y = 0;y <= fldsizeh[player];y++){
					if((fld[i + y * fldsizew[player] + player * 220] != 0) && (fld[(i - 1) + y * fldsizew[player] + player * 220] == 0)){
						tmp = i;
						do{
							if(tmp - 1 < 0) break;	//左端に到達
							j = fld[tmp + y * fldsizew[player] + player * 220];
							fld[tmp + y * fldsizew[player] + player * 220] = 0;
							fld[(tmp - 1) + y * fldsizew[player] + player * 220] = j;
							j = fldt[tmp + y * fldsizew[player] + player * 220];
							fldt[tmp + y * fldsizew[player] + player * 220] = 0;
							fldt[(tmp - 1) + y * fldsizew[player] + player * 220] = j;
							j = fldi[tmp + y * fldsizew[player] + player * 220];
							fldi[tmp + y * fldsizew[player] + player * 220] = 0;
							fldi[(tmp - 1) + y * fldsizew[player] + player * 220] = j;
							j = flds[tmp + y * fldsizew[player] + player * 220];
							flds[tmp + y * fldsizew[player] + player * 220] = 0;
							flds[(tmp - 1) + y * fldsizew[player] + player * 220] = j;
							tmp--;
						}while(fld[(tmp - 1) + y * fldsizew[player] + player * 220] == 0);
					}
				}
			}
			if(isRmovfield[player])	//→MOV FIELD
			for(i = fldsizew[player] - 2;i >= 0;i--){
				for(y = 0;y <= fldsizeh[player];y++){
					if((fld[i + y * fldsizew[player] + player * 220] != 0) && (fld[(i + 1) + y * fldsizew[player] + player * 220] == 0)){
						tmp = i;
						do{
							if(tmp + 1 >= fldsizew[player]) break;	//右端に到達
							j = fld[tmp + y * fldsizew[player] + player * 220];
							fld[tmp + y * fldsizew[player] + player * 220] = 0;
							fld[(tmp + 1) + y * fldsizew[player] + player * 220] = j;
							j = fldt[tmp + y * fldsizew[player] + player * 220];
							fldt[tmp + y * fldsizew[player] + player * 220] = 0;
							fldt[(tmp + 1) + y * fldsizew[player] + player * 220] = j;
							j = fldi[tmp + y * fldsizew[player] + player * 220];
							fldi[tmp + y * fldsizew[player] + player * 220] = 0;
							fldi[(tmp + 1) + y * fldsizew[player] + player * 220] = j;
							j = flds[tmp + y * fldsizew[player] + player * 220];
							flds[tmp + y * fldsizew[player] + player * 220] = 0;
							flds[(tmp + 1) + y * fldsizew[player] + player * 220] = j;
							tmp++;
						}while(fld[(tmp + 1) + y * fldsizew[player] + player * 220] == 0);
					}
				}
			}

		}
	}


	statc[player * 10 + 0]++;
	if(statc[player * 10 + 0] == (wait1[player] * 2) + 30){
		isLmovfield[player] = 0;
		isRmovfield[player] = 0;
		stat[player] = statc[player * 10 + 1];
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 2] = 0;
		if(repversw >= 48){
			statc[player * 10 + 1] = stat[player];
			stat[player] = 22;
		}
	}

}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.34 - フィールド上のブロックをひっくり返す
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
void stat180field(int player) {
	int i,j,x,y,l,k,tmp,waitA;

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if(repversw < 63){
		waitA = wait1[player] + 40;
	}else if(repversw < 65){
		waitA = wait1[player] * 2;
	}else{
		waitA = 20 + wait1[player] + (wait1[player] / 2);
	}

//	do180field[player] = 0;

	if((statc[player * 10 + 0] >= waitA) && (statc[player * 10 + 0] <= waitA + 50 + 15)){
		if(statc[player * 10 + 0] < waitA + 25){
			ofs_y[player] = ofs_y[player] + 2;
			ofs_y2[player] = ofs_y2[player] + 2;
		}
		if((statc[player * 10 + 0] >= waitA + 25) && (statc[player * 10 + 0] <= waitA + 50)){
			if(ofs_y[player] > 0) ofs_y[player] = ofs_y[player] - 10;
			ofs_y2[player] = ofs_y2[player] - 10;
		}
		if(statc[player * 10 + 0] == waitA + 50){	//処理
			ofs_y[player] = 0;
			statc[player * 10 + 2] = checkFieldTop(player);
			for(i = 0; i <= fldsizeh[player]; i++) {	// バッファへ
				for(j = 0; j < fldsizew[player]; j++) {
					fldbuf[j + i * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220];
					fldtbuf[j + i * fldsizew[player] + player * 220] = fldt[j + i * fldsizew[player] + player * 220];
					fldibuf[j + i * fldsizew[player] + player * 220] = fldi[j + i * fldsizew[player] + player * 220];
					fldsbuf[j + i * fldsizew[player] + player * 220] = flds[j + i * fldsizew[player] + player * 220];
				}
			}
			for(i = 0; i <= fldsizeh[player]; i++) {	// ひっくり返す
				for(j = 0; j < fldsizew[player]; j++) {
					if(i + statc[player * 10 + 2] > fldsizeh[player]) break;
					fld[j + (i + statc[player * 10 + 2]) * fldsizew[player] + player * 220] = fldbuf[j + (fldsizeh[player] - i) * fldsizew[player] + player * 220];
					fldt[j + (i + statc[player * 10 + 2]) * fldsizew[player] + player * 220] = fldtbuf[j + (fldsizeh[player] - i) * fldsizew[player] + player * 220];
					fldi[j + (i + statc[player * 10 + 2]) * fldsizew[player] + player * 220] = fldibuf[j + (fldsizeh[player] - i) * fldsizew[player] + player * 220];
					flds[j + (i + statc[player * 10 + 2]) * fldsizew[player] + player * 220] = fldsbuf[j + (fldsizeh[player] - i) * fldsizew[player] + player * 220];

				}
			}
		}
		if((statc[player * 10 + 0] > waitA + 50) && (statc[player * 10 + 0] < waitA + 50 + 15)){
			ofs_y2[player] = ofs_y2[player] + 20;
			if(ofs_y2[player] > 0) ofs_y2[player] = 0;
		}
		if(statc[player * 10 + 0] == waitA + 50 + 15){
			ofs_y2[player] = 0;
			PlaySE(2);
		}
	}

	statc[player * 10 + 0]++;
	if(statc[player * 10 + 0] > wait1[player] + waitA + 50 + 15){
		stat[player] = statc[player * 10 + 1];
		statc[player * 10] = 0;
		statc[player * 10 + 1] = 0;
		statc[player * 10 + 2] = 0;
		if(repversw >= 48){
			statc[player * 10 + 1] = stat[player];
			stat[player] = 22;
		}
	}

}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.35 - 上からブロックを消去
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statc[player * 10 + 0]：カウンタ
// statc[player * 10 + 1]：終了後に移動するステータス
// statc[player * 10 + 2]：消すライン数
// statc[player * 10 + 3]：カウンタ2
void statDelfromUpper(int player) {
	int i, j, k, waitA;

	padRepeat(player);
	if(ace_irs) doIRS2(player);	// ACE式IRS C7U1.5

	if((repversw < 63) || (gameMode[player] == 8)){
		waitA = -160;
	}else{
		waitA = -100 - (wait1[player] * 2);
	}

	if((statc[player * 10] > -100 + wait1[player]) && (statc[player * 10] < 0)){
		statc[player * 10]++;
		return;
	}
	if(statc[player * 10] == 0) {
		// どこから消すか決める
		statc[player * 10] = checkFieldTop(player) * 6;
		return;
	}

	// ブロックを消す
	if((statc[player * 10] % 6 == 0) && (statc[player * 10] >= 0)) {
		j = statc[player * 10] / 6;

		if((j >= 0) && (j < 22)) {
			for(i = 0; i < 10; i++) {
				if(fld[i + j * 10 + player * 220]) {
						objectCreate(player, 1, (i + 15 + 24 * player - 12 * maxPlay) * 8, (j + 3) * 8, (i - 5) * 120 + 20 - Rand(40), - 1900 + Rand(150), fld[i + j * 10 + player * 220], 100);

					fld[i + j * 10 + player * 220] = 0;
					fldt[i + j * 10 + player * 220] = 0;
					fldi[i + j * 10 + player * 220] = 0;
					flds[i + j * 10 + player * 220] = 0;
				}
			}
		}
		statc[player * 10 + 3]++;
	}
	if((statc[player * 10] >= 0) && ((statc[player * 10 + 3] >= statc[player * 10 + 2]) || ((statc[player * 10] / 6) >= 22)))
		statc[player * 10] = waitA;

	// アニメ終了
	if(statc[player * 10] == -100) {
		pinchCheck(player);
		pinchCheck2(player);
		stat[player] = statc[player * 10 + 1];
		for(i = 0; i < 10; i++) statc[player * 10 + i] = 0;
		for(i = 0; i < 22; i++) erase[i + player * 22] = 0;
		return;
	}

	statc[player * 10]++;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.EX - シャッフルフィールド
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statBanana(int player){
	int i,j,y,flag[10],tmp,chg[10],chgt[10],chgi[10],chgs[10];

	if(banana_c[player] == 0){	// 最初
		for(i = 0; i < 10; i++) flag[i] = 0;
		for(i = 0; i < 10; i++){
			do{
				tmp = rand(10,player);
			}while(flag[tmp] == 1);
			flag[tmp] = 1;
			banana_pos[i + 10 * player] = tmp;
		}
		banana_c[player]++;
	}else{
		y = banana_c[player] - 1;
		if(banana_c[player] <= 22){
			for(j = 0; j < fldsizew[player]; j++){
				chg[j] = fld[j + y * fldsizew[player] + player * 220];
				chgt[j] = fldt[j + y * fldsizew[player] + player * 220];
				chgi[j] = fldi[j + y * fldsizew[player] + player * 220];
				chgs[j] = flds[j + y * fldsizew[player] + player * 220];
			}
			for(j = 0; j < fldsizew[player]; j++){
				fld[j + y * fldsizew[player] + player * 220] = chg[banana_pos[j + 10 * player]];
				fldt[j + y * fldsizew[player] + player * 220] = chgt[banana_pos[j + 10 * player]];
				fldi[j + y * fldsizew[player] + player * 220] = chgi[banana_pos[j + 10 * player]];
				flds[j + y * fldsizew[player] + player * 220] = chgs[banana_pos[j + 10 * player]];
			}
		}
		if((y > 1) && (y <= 7))
			ExBltRect(79, 120 + 192 * player - 96 * maxPlay, 32, 0 , 50 * (count % 4) + (50 - ((y - 1) * 8)), 80, (y - 1) * 8);
		else if((y > 21) && (y <= 27))
			ExBltRect(79, 120 + 192 * player - 96 * maxPlay, 26 + (y * 8) - 50, 0 , 50 * (count % 4), 80, ((28 - y) * 8));
		else if((y > 7) && (y <= 21))
			ExBltRect(79, 120 + 192 * player - 96 * maxPlay, 24 + (y * 8) - 50, 0 , 50 * (count % 4), 80, 50);
		banana_c[player]++;
	}
	if(banana_c[player] > 32){
		isbanana[player] = 0;
		banana_c[player] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  擬似乱数のシード生成
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void GiziSRand(int player){
	int seed;

	seed = Rand(65536);
	randseed[player] = seed;
	first_seed[player] = seed;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  擬似乱数生成
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
int rand(int max,int player) {

		randseed[player] = ((randseed[player] * 673 + 994) / 10) % 100000;
		return abs_YGS2K(randseed[player]) % max;
}

// 絶対値を求める（YGS2Kにはデフォルトでは入っていないので）
int abs_YGS2K(int i){
	if(i < 0)
		return -i;
	else
		return i;
}

// 特定の座標にブロックが存在するか調べる
// 何も無ければ 0
// フィールド外は -1
// フィールドより上は -2
//　opt:1なら現在のステータス異常を考慮　操作中ブロック向け
//  fld[bx1 + by1 * fldsizew[player] + player * 220];
int getFieldBlock(int player, int bx1, int by1, int opt) {
	//フィールド外？
	if(((bx1 < 0) && (by1 > 0)) || ((bx1 > fldsizew[player] - 1) && (by1 > 0)) || (by1 > fldsizeh[player])) return -1;
	if(by1 < 0) return 0;

	if(opt){
		if(hidden[player] == 8) return 0;
		else if((isdark[player]) && (item_timer[player] <= 420)) return 0;
		else if(isblind[player]) return 0;
		else if((isxray[player]) && (xray_counter[player] % p_xray_frame != bx1)) return 0;
		else if((hidden[player] >= 9) && (fldt[bx1 + by1 * fldsizew[player] + player * 220] == 0)) return 0;
	}

	return (fld[bx1 + by1 * fldsizew[player] + player * 220]);
}

int getFieldBlock2(int player, int bx1, int by1) {
	if((bx1 < 0) || (bx1 > 10 - 1) || (by1 > fldsizeh[player])) return -1;
	else return (tomoyo_rise_fld[bx1 + by1 * 10 + player * 200]);
}

int getFieldBlock3(int player, int bx1, int by1) {
	if((bx1 < 0) || (bx1 > 10 - 1) || (by1 > fldsizeh[player])) return 0 - (1 * (gameMode[player] == 5));
	else return (fldu[bx1 + by1 * 10 + player * 220]);
}
// gamemodefont.png用
int getModeAdd(int player){
	if(gameMode[player] == 0) return novice_mode[player];
	else if((gameMode[player] == 1) || (gameMode[player] == 2)) return enable_grade[player];
	else if(gameMode[player] == 3) return devil_minus[player];
	else if(gameMode[player] == 6) return (start_stage[player] >= 27)+(start_stage[player] >= 45)+(start_stage[player] >= 69)+(start_stage[player] >= 100);
	else if(gameMode[player] == 7) return anothermode[player];
	else if(gameMode[player] == 9){
		if(relaymode[player]) return 4;
		else return std_opt[player];
	}else if(gameMode[player] == 10) return ori_opt[player];
	else return 0;
}
// フィールドの最上段を返す
int checkFieldTop(int player) {
	int i, j;

	for(i = 0; i <= fldsizeh[player]; i++)
		for(j = 0; j < fldsizew[player]; j++)
			if(fld[j + i * fldsizew[player] + player * 220])
				return i;

	// 空っぽの場合
	return 22;
}

// フィールドの最上段を返す
// x軸を指定出来るバージョン
int checkFieldTop2(int player,int x) {
	int i;

	for(i = 0; i <= fldsizeh[player]; i++)
		if(fld[x + i * fldsizew[player] + player * 220])
			return i;

	// 空っぽの場合
	return 22;
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//   表示するアイテム名を決定
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void scanItem(int player) {
	int i, j;

	item_name[player] = 0;

	// フィールドを調べる
	for(i = fldsizeh[player]; i >= 0; i--)
		for(j = 0; j < fldsizew[player]; j++)
			if((fldi[j + i * fldsizew[player] + player * 220] != 0) && (fldi[j + i * fldsizew[player] + player * 220] < fldihardno)) {
				item_name[player] = fldi[j + i * 10 + player * 220];
			}

	// NEXTを調べる
	for(i = 0; i < 6; i++)
		if((item_nblk[i + player * 6] != 0) && (item_nblk[i + player * 6] < fldihardno)) {
			item_name[player] = item_nblk[i + player * 6];
		}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  パッドリピート入力処理
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void padRepeat(int player) {
	if(getPressState(player, 2) && getPressState(player, 3)) {
		mp[player] = 0;
		mpc[player] = 0;
	} else if(getPressState(player, 2)) {
		if(mp[player] != 2) {
			mp[player] = 2;
			mpc[player] = 0;
		}
		mpc[player]++;
	} else if(getPressState(player, 3)) {
		if(mp[player] != 3) {
			mp[player] = 3;
			mpc[player] = 0;
		}
		mpc[player]++;
	} else {
		mp[player] = 0;
		mpc[player] = 0;
	}
}
void padRepeat2(int player) { // hoge 上下入力バージョン
	if(getPressState(player, 0) && getPressState(player, 1)) {
		mp2[player] = 0;
		mpc2[player] = 0;
	} else if(getPressState(player, 0)) {
		if(mp2[player] != 1) {
			mp2[player] = 1;
			mpc2[player] = 0;
		}
		mpc2[player]++;
	} else if(getPressState(player, 1)) {
		if(mp2[player] != 2) {
			mp2[player] = 2;
			mpc2[player] = 0;
		}
		mpc2[player]++;
	} else {
		mp2[player] = 0;
		mpc2[player] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  キー状態の取得
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// ジョイスティック周りを変更 #1.60c7
int getPressState(int player, int key) { // パッドボタン割り当て対応 #1.60c
	int jtmp;	// ジョイスティック入力
	int ktmp;	// キーボード入力
	int pl;

	// シングルモードでは2P側の入力を常にカット
	if(maxPlay < player) return 0;

	if((isremote[player]) && (gameMode[player] == 4)) {
		pl = 1 - player;	// リモコン
	} else {
		pl = player;	// 通常
	}


	// リプレイ
	if(playback) {
		// 入力をリプレイデータから読み取る
		ktmp = (replayData[time2[pl] + pl* 60 * 60 * 20] & (1 << key)) / (1 << key);

		if((!pl) || (gameMode[0] == 4))
			return ktmp;
		else
			return 0;
	}
	// 通常プレイ中
	else {
		SelectJoyStick(pl);

		// ジョイスティックの入力を読み取る
		jtmp = IsPressJoyKey(joykeyAssign[key + 10 * pl]);

		// キーボードの入力を読み取る
		ktmp = IsPressKey(keyAssign[key + pl * 10]);

		if(cpu_flag[pl])
			return cp_input[key + pl * 10];
		else if(!keyAssign[key + pl * 10])
			return jtmp;
		else
			return (jtmp | ktmp);
	}
}

int getPushState(int player, int key) { // パッドボタン割り当て対応 #1.60c
	int jtmp;	// ジョイスティック入力
	int ktmp;	// キーボード入力
	int pl;

	// シングルモードでは2P側の入力を常にカット
	if(maxPlay < player) return 0;

	if((isremote[player]) && (gameMode[player] == 4)) {
		pl = 1 - player;	// リモコン
	} else {
		pl = player;	// 通常
	}
	// リプレイ
	if(playback) {
		// 入力をリプレイデータから読み取る
		ktmp = (replayData[time2[pl] + pl* 60 * 60 * 20] & (1 << (key + 8))) / (1 << (key + 8));

		if((!pl) || (gameMode[0] == 4))
			return ktmp;
		else
			return 0;
	}
	// 通常プレイ中
	else {
		SelectJoyStick(pl);

		// ジョイスティックの入力を読み取る
		jtmp = IsPushJoyKey(joykeyAssign[key + 10 * pl]);

		// キーボードの入力を読み取る
		ktmp = IsPushKey(keyAssign[key + pl * 10]);

		if(cpu_flag[pl])
			return cp_input[key + pl * 10];
		else
			return (jtmp | ktmp);
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ブロックのあたり判定
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
int judgeBlock(int player, int bx1, int by1, int kind, int rotate) {
	int		i, bx2, by2; //, result

	if(IsBig[player]) {
		// BIGの場合は専用の判定処理を行う #1.60c7m6
		return judgeBigBlock(player, bx1, by1, kind, rotate);
	}

	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i]);
		}else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i]);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]);
		}
		if((by2 < 2) && (heboGB[player] == 2)) {
			return -1;
		}

		if(((bx2 < 0) || (bx2 >= fldsizew[player]) ||
			(by2 > fldsizeh[player])) || ((fld[bx2 + by2 * fldsizew[player] + player * 220] != 0) && (by2 >= 0))) {
			return -1;
		}
	}

	return 0;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ブロックを設置する fldsize対応 #1.60c7
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void setBlock(int player, int bx1, int by1, int kind, int rotate) {
	int		i, bx2, by2;
	int		puted;

	if(IsBig[player]) {
		// BIGの場合は専用の設置処理を行う #1.60c7m6
		setBigBlock(player, bx1, by1, kind, rotate);
		return;
	}

	puted = 0;

	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i]);
		}else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i]);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]);
		}
		if((bx2 >= 0) && (bx2 < fldsizew[player]) && (by2 >= 0+(1*((repversw <= 27) || (!spawn_y_type)))) && (by2 <= fldsizeh[player])) {
			if((by2 >= 2) || (repversw <= 27))puted++;

			fld[bx2 + by2 * fldsizew[player] + player * 220] = c_cblk_r[player] + 1;
			if((c_cblk_r[player] + 1 == 9) && (heboGB[player] == 1))
				fld[bx2 + by2 * fldsizew[player] + player * 220] = 10;

			// シャドウタイマーセット #1.60c7j5
			if((hidden[player] == 8) && (m_roll_blockframe))
				fldt[bx2 + by2 * fldsizew[player] + player * 220] = -60;
			else if(hidden[player] == 8)
				fldt[bx2 + by2 * fldsizew[player] + player * 220] = 0;
			else if(shadow_timer_flag[player])
				fldt[bx2 + by2 * fldsizew[player] + player * 220] = p_shadow_timer;
			else
				fldt[bx2 + by2 * fldsizew[player] + player * 220] = -1;

			// アイテム設置 #1.60c7n1
			fldi[bx2 + by2 * fldsizew[player] + player * 220] = item[player];
			flds[bx2 + by2 * fldsizew[player] + player * 220] = bdowncnt[player] + 1;
		}
	}

	// 完全に画面外に置いたら死亡 #1.60c7s2
	if(((spawn_y_type) || (repversw >= 57)) && (!puted)) {
		setGameOver(player);
	}
}

/* BIG用当たり判定 #1.60c7m6 */
int judgeBigBlock(int player, int bx1, int by1, int kind, int rotate) {
	int		i, bx2, by2; //, result
	int		k, l, bx3, by3;

	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i] * 2);
		}else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i] * 2);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2);
		}

		/* 4ブロック分調べる */
		for(k = 0; k < 2; k++)for(l = 0; l < 2; l++) {
			bx3 = bx2 + k;
			by3 = by2 + l;
			if((by3 < 2) && (heboGB[player] == 2)) {
				return -1;
			}
			if( ((bx3 < 0) || (bx3 >= fldsizew[player]) || (by3 > fldsizeh[player])) ||
			    ((fld[bx3 + by3 * fldsizew[player] + player * 220] != 0) && (by3 >= 0)) ) {
				return -1;
			}
		}
	}

	return 0;
}

/* BIG用設置処理 #1.60c7m6 */
void setBigBlock(int player, int bx1, int by1, int kind, int rotate) {
	int		i, bx2, by2;
	int		k, l, bx3, by3;
	int		puted;

	puted = 0;

	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i] * 2);
		}else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i] * 2);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2);
		}

		/* 4ブロック分設置する */
		for(k = 0; k < 2; k++)for(l = 0; l < 2; l++) {
			bx3 = bx2 + k;
			by3 = by2 + l;

		if((bx3 >= 0) && (bx3 < fldsizew[player]) && (by3 >= 0+(1*((repversw <= 27) || (!spawn_y_type)))) && (by3 <= fldsizeh[player])) {
				if((repversw <= 2) || (by3 >= 2)) puted = 1;

				fld[bx3 + by3 * fldsizew[player] + player * 220] = c_cblk_r[player] + 1;
				if((c_cblk_r[player] + 1 == 9) && (heboGB[player] == 1))
					fld[bx3 + by3 * fldsizew[player] + player * 220] = 10;

				if((hidden[player] == 8) && (m_roll_blockframe))	//TI風消えロールにも使用
					fldt[bx3 + by3 * fldsizew[player] + player * 220] = -60;
				else if(hidden[player] == 8)
					fldt[bx3 + by3 * fldsizew[player] + player * 220] = 0;
				else if(shadow_timer_flag[player])
					fldt[bx3 + by3 * fldsizew[player] + player * 220] = p_shadow_timer;
				else
					fldt[bx3 + by3 * fldsizew[player] + player * 220] = -1;

				// アイテム設置 #1.60c7n1
				fldi[bx3 + by3 * fldsizew[player] + player * 220] = item[player];
				flds[bx3 + by3 * fldsizew[player] + player * 220] = bdowncnt[player] + 1;
			}
		}
	}

	// 完全に画面外に置いたら死亡 #1.60c7s2
	if(((spawn_y_type) || (repversw >= 57)) && (!puted)) {
		setGameOver(player);
	}
}
// ブロックを消す　MOVE ERASED せり上がり用
void removeBlock(int player, int bx1, int by1, int kind, int rotate) {
	int		i, bx2, by2;

	if(IsBig[player]) {
		// BIGの場合は専用処理を行う #1.60c7m6
		removeBigBlock(player, bx1, by1, kind, rotate);
		return;
	}

	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i]);
		}else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i]);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]);
		}
		if( ( bx2 >= 0 ) && ( bx2 <= 9 ) && ( by2 >= 0 + (!spawn_y_type) ) && ( by2 <= fldsizeh[player] ) ) {
			fld[bx2 + by2 * fldsizew[player] + player * 220] = 0;
			fldt[bx2 + by2 * fldsizew[player] + player * 220] = 0;

		}
	}
}

// BIG用
void removeBigBlock(int player, int bx1, int by1, int kind, int rotate) {
	int		i, bx2, by2;
	int		k, l, bx3, by3;

	for(i = 0 ; i < 4 ; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i] * 2);
		}else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i] * 2);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2);
		}

		/* 4ブロック分消す */
		for(k = 0; k < 2; k++)for(l = 0; l < 2; l++) {
			bx3 = bx2 + k;
			by3 = by2 + l;

			if( ( bx3 >= 0 ) && ( bx3 <= 9 ) && ( by3 >= 0 + (!spawn_y_type) ) && ( by3 <= fldsizeh[player] ) ) {
				fld[bx3 + by3 * 10 + player * 220] = 0;
				fldt[bx3 + by3 * 10 + player * 220] = 0;
			}
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  テストメニュー#1.60c7i4
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void testmenu(void) {
	int i, j, cursor, mode, param;
	cursor = 0;
	mode = 0;
	param = 0;

	loop {
		ClearSecondary();

		// メインメニュー
		if( mode == 0 ) {
			param = 0;

			// メニュー描画
			printFont(1, 1, "TEST MENU", 4);
			printFont(1, 3 + cursor, "b", fontc[rots[0]]);

			printFont(2, 3, "[GRAPHIC TEST]", (cursor == 0) * fontc[rots[0]]);
			printFont(2, 4, "[RANKING ERASE]",(cursor == 1) * fontc[rots[0]]);
			printFont(2, 5, "[RESET]",        (cursor == 2) * fontc[rots[0]]);

			// キー入力
			KeyInput();

			padRepeat2(0);
			// ↑
			if( (mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)) )
			if( getPressState(0, 0) ) {
				PlaySE(5);
				cursor--;
				if(cursor < 0) cursor = 2;
			}
			// ↓
			if( (mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)) )
			if( getPressState(0, 1) ) {
				PlaySE(5);
				cursor++;
				if(cursor > 2) cursor = 0;
			}

			// Aで決定
			if( getPushState(0, 4) ) {
				PlaySE(10);
				mode = cursor + 1;
			}

			// Bで戻る
			if( getPushState(0, 5) ) {
				return;
			}
		}
		// GRAPHIC TEST
		else if( mode == 1 ) {
			ExBltFast(param, 0, 0);

			// キー入力
			KeyInput();

			padRepeat(0);
			// ←
			if((mpc[0] == 1) || ((mpc[0] > tame1) && (mpc[0] % tame2 == 0)) || (getPressState(0, 6)))
			if( getPressState(0, 2) ) {
				param--;

				// 一時バッファを描画しない
				if( (param == 9) || (param == 23) ) param--;

				// 未使用プレーンを描画しない
				if( param == 30 ) param = 29;
				//if( (param == 26) || (param == 27) ) param = 25;

				// マイナスになっていたら79にする
				if( param < 0 ) param = 89;
			}
			// →
			if((mpc[0] == 1) || ((mpc[0] > tame1) && (mpc[0] % tame2 == 0)) || (getPressState(0, 6)))
			if( getPressState(0, 3) ) {
				param++;

				// 未使用プレーンを描画しない
				//if( (param == 26) || (param == 27) ) param = 28;
				if( param == 30 ) param = 31;

				// 一時バッファを描画しない
				if( (param == 9) || (param == 23) ) param++;

				// 79以上になっていたら0にする
				if( param > 89 ) param = 0;
			}

			if( !getPressState(0, 7) ) {
				sprintf(string[0],"NO.%02d B:EXIT D:HIDE",param);
				printFont(0, 29, string[0], 0);
			}

			// Bで戻る
			if( getPushState(0, 5) ) {
				mode = 0;
			}
		}
		// RANKING ERASE
		else if( mode == 2 ) {
			if(param == 0) {
				// 確認
				printFont(1, 1, "ERASE RANKING?", 4);

				printFont(1, 3, "A+C : YES", 2);
				printFont(1, 4, "B   : NO",  1);
			} else {
				// 消去済み
				printFont(1, 1, "ERASED", 4);

				printFont(1, 4, "B   : EXIT",1);
			}

			// キー入力
			KeyInput();

			// A+Cで決定
			if( (!param) && (getPressState(0, 4)) && (getPressState(0, 6)) ) {
				PlaySE(10);
				RankingInit();
				RankingInit2();
				RankingInit3();
				param = 1;
			}

			// Bで戻る
			if( getPushState(0, 5) ) {
				mode = 0;
			}
		}
		// RESET
		else if( mode == 3 ) {
			printFont(1, 1, "RESET GAME?", 4);

			printFont(1, 3, "A+C : YES", 2);
			printFont(1, 4, "B   : NO",  1);

			// キー入力
			KeyInput();

			// A+Cで決定
			if( (getPressState(0, 4)) && (getPressState(0, 6)) ) {
				PlaySE(10);
				ClearSecondary();
				printFont(1, 1, "PLEASE WAIT...", 2);
				restart = 1;
				return;
			}

			// Bで戻る
			if( getPushState(0, 5) ) {
				mode = 0;
			}
		}

		spriteTime();
	}/* loop */
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  初期設定
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void initialize(void) {
	int i, j;
	SetFillColor(0);
	ClearSecondary();
	halt;

	// SetConstParam("Caption", "Now Loading...");

	for ( int ii = 1 ; ii <= 5 ; ii ++ )
	{
		TextSize(ii, 12);
		TextBackColorDisable(ii);
	}

	TextLayerOn(1, 10, 220);
	TextOut(1, version);
	halt;


	//
	//TextLayerOn(2, 10, 75);
    //TextOut(2, "If you are English\nuser,please read\n[for_english_users.txt]\nwell.");
	//halt;


	TextLayerOn(3, 10, 10);
	TextOut(3, "Config Loading");
	halt;

	if(LoadConfig()) {	//CONFIG.SAVより設定をロード
		readdef::readdef();
		LoadConfig();
	}

	hnext[0] = dispnext;	// #1.60c7o8
	hnext[1] = dispnext;	// #1.60c7o8
	versus_rot[0] = rots[0];
	versus_rot[1] = rots[1];

	// 画面比率に応じて画像解像度も変える #1.60c7p9ex
	if ( screenMode < 2 || screenMode > 6 ) {
		setDrawRate(1);
	} else {
		setDrawRate(2);
	}

	LoadGraphics("loading.png", 88, 0);		// Loading表示
		i = Rand(5);
	if ( getDrawRate() != 1 )
		j = Rand(2);
	else
		j = 0;

	// グラフィック読み込み
	TextLayerOn(4, 10, 23);
	TextOut(4, "Graphics Loading");
	ExBltFastRect(88, 160, 0,160 * i,240 * j,160,240);
	halt;
	loadGraphics(maxPlay);

	// 効果音読み込み
	if(se) {
		TextLayerOn(1, 10, 36);
		TextOut(1, "Sound Effect Loading");
		ExBltFastRect(88, 160, 0,160 * i,240 * j,160,240);
		halt;
		loadWaves();	// #1.60c7o5
	}

	for(i = 0; i < 50; i++) se_play[i] = 0;

	// BGM読み込み
	if(bgm) {
		TextLayerOn(5, 10, 49);
		TextOut(5, "BGM Loading");
		ExBltFastRect(88, 160, 0,160 * i,240 * j,160,240);
		halt;

		if(wavebgm != 0) {
			loadBGM();	// #1.60c7s6
		} else {
			LoadMIDI("bgm/bgm.mid");
			PlayMIDI();
		}
	}

	for ( int ii = 1 ; ii <= 5 ; ii ++ )
	{
		TextLayerOff(ii);
	}

	if(ranking_type==0){
		i = RankingLoad();
		if(i == 1) RankingInit();
		if(i == 2) RankingConvert();
	}else if(ranking_type==1){
		RankingAlloc();
		i = RankingLoad2();
		if(i == 1) RankingInit2();
	}else{
		RankingAlloc3();
		i = RankingLoad3();
		if(i == 1) RankingInit3();
	}
	// ジョイスティックのボタンは3つ以上使いますよー
	for(i = 0; i < 2; i++) {
		SelectJoyStick(i);
		SetJoyButtonMax(16);
	}

	// 連続スナップ取得領域設定
	if((capx < 0) || (capx > 320)) capx = capx % 320;
	if((capy < 0) || (capy > 240)) capy = capy % 240;
	if(capw < 1) capw = 1;
	if(caph < 1) caph = 1;
	if(capx + capw > 320) capw = 320 - capx;
	if(capy + caph > 240) caph = 240 - capy;

	// スタッフロール用データを初期化
//	staffInit();

	// セクションタイムランキング読み込み
	if( ST_RankingLoad() ) {
		ST_RankingInit();
	}

	PlayerdataLoad();

	// SetConstParam("Caption", "HEBORIS C7-EX YGS2K+");
	/* ここからポーズ有効 #1.60c7p9ex */
	// SetConstParam("EnablePause", 1);
}

/* グラフィック読み込み */
// initializeから独立 #1.60c7o5
// players : プレイする人数(maxPlayの代わり)
void LoadGraphics(char *nameStr, int p1, int p2) {
	if ( getDrawRate() == 1 )
		sprintf(string[0], "res/graphics/lowDetail%s", nameStr);
	else
		sprintf(string[0], "res/graphics/highDetail/%s", nameStr);

	LoadBitmap(string[0], p1, p2);
}

void LoadTitle(){
	if(!title_mov_f){		//タイトルは静止画
		LoadGraphics("title.png", 8, 0);
		if ( getDrawRate() == 1 ){
			LoadBitmap("res/graphics/title/logo_low.png", 7,0);
		}else{
			LoadBitmap("res/graphics/title/logo_hi.png", 7,0);
		}
		SetColorKeyRGB(7,0,0,0);
	}else{					//動画
		if ( getDrawRate() == 1 ){
			LoadBitmap("res/graphics/title/tmov_low.png", 8,0);
			LoadBitmap("res/graphics/title/logo_low.png", 7,0);
		}else{
			LoadBitmap("res/graphics/title/tmov_hi.png" , 8,0);
			LoadBitmap("res/graphics/title/logo_hi.png", 7,0);
		}
		SetColorKeyRGB(7,0,0,0);
	}
}

void LoadBackground(char *nameStr, int p1, int p2) {
	if(skip_viewbg) return;

	if ( getDrawRate() == 1 )
		sprintf(string[0], "res/bg/lowDetail/%s", nameStr);
	else
		sprintf(string[0], "res/bg/highDetail/%s", nameStr);

	LoadBitmap(string[0], p1, p2);
}

void loadGraphics(int players) {
	int i, j, k, tr;

	/* プレーン0にメダルを読み込み #1.60c7m9 */
	LoadGraphics("medal.png", 0, 0);
	/* プレーン56にTIメダルを読み込み #1.60c7m9 */
	LoadGraphics("medal_ti.png", 56, 0);

	// ブロック絵はプレーン40～43に移転しました #1.60c7o8

	/* プレーン1にフォントを読み込み */
	LoadGraphics("hebofont.png", 1, 0);
	SetColorKeyPos(1, 0, 0);

	/* プレーン2にフィールドを読み込み */
	LoadGraphics("hebofld.png", 2, 0);
	SetColorKeyRGB(2,255,255,255);

	/* プレーン3に各種スプライトを読み込み */
	LoadGraphics("hebospr.png", 3, 0);
	SetColorKeyRGB(3,0,0,0);
//	SetColorKeyPos(3, 0, 0);
//	EnableBlendColorKey(3, 1);

	/* プレーン4～6, 24にフィールド背景を読み込み */
	LoadGraphics("heboflb1.png", 4, 0);
	LoadGraphics("heboflb2.png", 5, 0);
	LoadGraphics("heboflb3.png", 6, 0);
	LoadGraphics("heboflb0.png", 24, 0);

	loadBG(players,0); //背景および半透明処理部を独立 C7T2.5EX
	/* プレーン10～にバックを読み込み */

	/* プレーン7にタイトルロゴを読み込み */
//	LoadGraphics("logo.png", 7, 0);
//	SetColorKeyPos(7, 0, 0);

//	/* プレーン8にタイトル背景を読み込み */
	LoadTitle();
//	LoadGraphics("title.png", 8, 0);

	/* プレーン22に小文字大文字フォントを読み込み #1.60c7o4 */
	LoadGraphics("hebofont4.png", 22, 0);

	/* プレーン25にモード選択時のメッセージを読み込み  */
	LoadGraphics("text.png", 25, 0);

	SetColorKeyRGB(25, 0, 0, 0);

	/* プレーン26に段位表示画像を読み込み #1.60c7t2.2 */
	LoadGraphics("grade.png", 26, 0);
	SetColorKeyRGB(26,255,0,255);

	/* プレーン27にミラーエフェクト画像を読み込み #1.60c7t2.2 */
	LoadGraphics("mirror_effect_TAP.png", 27, 0);
	//SetColorKeyRGB(27,255,0,255);

	/* プレーン28にアイテム名を読み込み #1.60c7o4 */
	LoadGraphics("item.png", 28, 0);
	SetColorKeyRGB(28,255,0,255);

	/* プレーン29に操作中ブロックの周り枠を読み込み #1.60c7o5 */
	LoadGraphics("guide.png", 29, 0);
	SetColorKeyRGB(29,0,0,0);

	/* プレーン31にフォント(大)を読み込み */
	LoadGraphics("hebofont3.png", 31, 0);
	SetColorKeyRGB(31,0,0,0);

	/* ブロック消去エフェクトを読み込み */
	if(breakti) {
		LoadGraphics("break0.png", 32, 0);
		LoadGraphics("break1.png", 33, 0);
		LoadGraphics("break2.png", 34, 0);
		LoadGraphics("break3.png", 35, 0);
		LoadGraphics("break4.png", 36, 0);
		LoadGraphics("break5.png", 37, 0);
		LoadGraphics("break6.png", 38, 0);
		LoadGraphics("break7.png", 39, 0);
	SetColorKeyRGB(32,   0, 0,   0);
	SetColorKeyRGB(33,   0, 0,   0);
	SetColorKeyRGB(34,   0, 0,   0);
	SetColorKeyRGB(35,   0, 0,   0);
	SetColorKeyRGB(36,   0, 0,   0);
	SetColorKeyRGB(37,   0, 0,   0);
	SetColorKeyRGB(38,   0, 0,   0);
	SetColorKeyRGB(39,   0, 0,   0);
	} else {
		LoadGraphics("break0_tap.png", 32, 0); // 黒ブロック追加 #1.60c7i5
		LoadGraphics("break1_tap.png", 33, 0);
		LoadGraphics("break2_tap.png", 34, 0);
		LoadGraphics("break3_tap.png", 35, 0);
		LoadGraphics("break4_tap.png", 36, 0);
		LoadGraphics("break5_tap.png", 37, 0);
		LoadGraphics("break6_tap.png", 38, 0);
		LoadGraphics("break7_tap.png", 39, 0);
	SetColorKeyRGB(32, 255, 0, 255);
	SetColorKeyRGB(33, 255, 0, 255);
	SetColorKeyRGB(34, 255, 0, 255);
	SetColorKeyRGB(35, 255, 0, 255);
	SetColorKeyRGB(36, 255, 0, 255);
	SetColorKeyRGB(37, 255, 0, 255);
	SetColorKeyRGB(38, 255, 0, 255);
	SetColorKeyRGB(39, 255, 0, 255);
	}

	/* プレーン40～46にブロック絵を読み込み #1.60c7o8 */
	LoadGraphics("heboblk0.png", 40, 0);	// TGM
	LoadGraphics("heboblk1.png", 41, 0);	// TI & ARS & ARS2
	LoadGraphics("heboblk2.png", 42, 0);	// WORLD & WORLD2
	LoadGraphics("heboblk3.png", 43, 0);	// WORLD3

	/* プレーン44にミッションモード用画像を読み込み */
	LoadGraphics("heboris_road.png", 44, 0);
	SetColorKeyRGB(44, 0, 0, 0);

	/* プレーン45にライン強制消去用画像を読み込み */
	LoadGraphics("del_field.png", 45, 0);
	SetColorKeyRGB(45, 0, 0, 0);

	/* プレーン46にプラチナブロックとアイテム絵を読み込み */
	LoadGraphics("heboblk_sp.png", 46, 0);

	/* プレーン47～53に花火を読み込み */
	LoadGraphics("hanabi_red.png",       47, 0);
	LoadGraphics("hanabi_orange.png",    48, 0);
	LoadGraphics("hanabi_yellow.png",    49, 0);
	LoadGraphics("hanabi_green.png",     50, 0);
	LoadGraphics("hanabi_waterblue.png", 51, 0);
	LoadGraphics("hanabi_blue.png",      52, 0);
	LoadGraphics("hanabi_purple.png",    53, 0);
	SetColorKeyRGB(47, 0, 0, 0);
	SetColorKeyRGB(48, 0, 0, 0);
	SetColorKeyRGB(49, 0, 0, 0);
	SetColorKeyRGB(50, 0, 0, 0);
	SetColorKeyRGB(51, 0, 0, 0);
	SetColorKeyRGB(52, 0, 0, 0);
	SetColorKeyRGB(53, 0, 0, 0);

	/* プレーン54にアイテムゲージを読み込み */
	LoadGraphics("item_guage.png",       54, 0);
	SetColorKeyRGB(54, 255, 0, 255);

	/* プレーン55に回転ルール性能指標を読み込み */
	LoadGraphics("rot.png",              55, 0);
	SetColorKeyRGB(55, 255, 0, 255);

	/* プラチナブロック消去エフェクトを読み込み */
	LoadGraphics("perase1.png", 57, 0);
	LoadGraphics("perase2.png", 58, 0);
	LoadGraphics("perase3.png", 59, 0);
	LoadGraphics("perase4.png", 60, 0);
	LoadGraphics("perase5.png", 61, 0);
	LoadGraphics("perase6.png", 62, 0);
	LoadGraphics("perase7.png", 63, 0);

	SetColorKeyRGB(57, 0, 0, 0);
	SetColorKeyRGB(58, 0, 0, 0);
	SetColorKeyRGB(59, 0, 0, 0);
	SetColorKeyRGB(60, 0, 0, 0);
	SetColorKeyRGB(61, 0, 0, 0);
	SetColorKeyRGB(62, 0, 0, 0);
	SetColorKeyRGB(63, 0, 0, 0);

	LoadGraphics("heboblk0B.png", 64, 0);

	LoadGraphics("shootingstar.png", 65, 0);
	SetColorKeyRGB(65, 0, 0, 0);

	/* TI式ミラー演出画像を読み込み */
	LoadGraphics("fldmirror01.png", 66, 0);
	LoadGraphics("fldmirror02.png", 67, 0);
	LoadGraphics("fldmirror03.png", 68, 0);
	LoadGraphics("fldmirror04.png", 69, 0);

	SetColorKeyRGB(66, 0, 0, 0);
	SetColorKeyRGB(67, 0, 0, 0);
	SetColorKeyRGB(68, 0, 0, 0);
	SetColorKeyRGB(69, 0, 0, 0);

	/* スタッフロールの画像を読み込み */
	LoadGraphics("staffroll.png", 70, 0);
	SetColorKeyRGB(70, 0, 0, 0);

	LoadGraphics("heboblk4_5.png", 73, 0);

	LoadGraphics("fade.png", 72, 0);
	SetColorKeyRGB(72, 255, 255, 255);

	LoadGraphics("heboblk_old.png", 74, 0);

	LoadGraphics("tomoyo_eh_fade.png", 75, 0);
	SetColorKeyRGB(75, 255, 0, 255);

	LoadGraphics("heboblk_big.png", 76, 0);
	LoadGraphics("line.png", 77, 0);//ランキングのライン
	SetColorKeyRGB(77, 0, 0, 0);

	LoadGraphics("laser.png", 78, 0);
	SetColorKeyRGB(78, 255, 0, 255);

	LoadGraphics("shuffle_field_effect.png", 79, 0);
	SetColorKeyRGB(79, 255, 0, 255);

	LoadGraphics("heboblk6.png", 80, 0);

	LoadGraphics("text2.png", 81, 0);
	SetColorKeyRGB(81, 0, 0, 0);

	LoadGraphics("itemerase.png", 82, 0);
	SetColorKeyRGB(82,0,0,0);

	LoadGraphics("heboblk_sp2.png", 83, 0);

	LoadGraphics("rotstext.png", 84, 0);
	SetColorKeyRGB(84, 0, 0, 0);
	LoadGraphics("hebofont5.png", 85, 0);
	SetColorKeyRGB(85, 172, 136, 199);

	LoadGraphics("gamemodefont.png", 86, 0);
	SetColorKeyRGB(86, 0, 0, 0);

	LoadGraphics("rollmark.png", 87, 0);
	SetColorKeyRGB(87, 0, 0, 0);

	//プレーン88番使用中…

	LoadGraphics("itemGra.png", 89, 0);

//	EnableBlendColorKey(85, 1);
	/* 050825 画面モード拡張改造部分-- ここから */
	/* x倍拡大用サーフェス*/
	// snapshot用に、拡大しない場合もダミープレーン作成 #1.60c
	if((screenMode == 0) || (screenMode == 1)) { sw = 320; sh = 240; }
	else if(screenMode == 4) { sw = 800; sh = 600; }
	else if(screenMode == 5) { sw = 1024; sh = 768; }
	else if(screenMode == 6) { sw = 1280; sh = 960; }
	else if((screenMode == 2) || (screenMode == 3) || (screenMode > 6)) { sw = 640; sh = 480; }
	/* ここまで */
	ExCreateSurface(9, sw, sh);

	/* プレーン23に鏡像のためのバッファ */
//	if(mirror) {
		ExCreateSurface(23, 320, 240);
//	}

	/* 背景半透明処理		…はloadBGへ移動 C7T2.5EX */


	// タイトル画像を暗くする #1.60c7o5
	if(background == 2) {
		ExCreateSurface(30, 320, 240);
		SwapToSecondary(30);
		ExBltFastRect(8, 0, 0,0,0,320,240);

		for(i = 0; i < 3; i++)
			BlendExBlt(24, (i % 3) * 120, 0, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);

		SwapToSecondary(30);
	}
}

/* 背景読み込みと半透明部分の埋め込み C7T2.5EX */
//SINGLE-DUAL台の変更時に読み直す必要があるもののみ
void loadBG(int players,int vsmode){
		int i, j, k, tr,max;
		int movframe, framemax, tmp1, tmp2;

	/* プレーン10～にバックを読み込み */
	LoadBackground("back01.png", 10, 0);
	LoadBackground("back02.png", 11, 0);
	LoadBackground("back03.png", 12, 0);
	LoadBackground("back04.png", 13, 0);
	LoadBackground("back05.png", 14, 0);
	LoadBackground("back06.png", 15, 0);
	LoadBackground("back07.png", 16, 0);
	LoadBackground("back08.png", 17, 0);
	LoadBackground("back09.png", 18, 0);
	LoadBackground("back10.png", 19, 0);
	LoadBackground("back11.png", 20, 0);
	LoadBackground("back12.png", 21, 0);

	if(vsbg == 1)
		LoadBackground("back_vs.png", 71, 0);
	else
		LoadBackground("back01.png", 71, 0);

	max = 22;
	EnableBlendColorKey(3, 1);

	/* 背景半透明処理 */
	if(!skip_viewbg) {
		for(i = 10; i < 72; i++) {
			if(i >= max){	//back12(21)の次は対戦用背景(71)
				i = 71;
				players = 1;	//対戦用なのでDUAL台
			}

			if(i <= 21) {
				// 普通背景
				if(back_mov_f[i - 10] == 0)
					framemax = 1;
				else
					framemax = back_mov_f[i - 10];
			} else {
				// 対戦背景
				if(back_mov_f[12] == 0)
					framemax = 1;
				else
					framemax = back_mov_f[12];
			}

			SwapToSecondary(i);

			for(movframe = 0; movframe < framemax; movframe++) {
				tmp1 = ((movframe / 10) * 320);
				tmp2 = ((movframe % 10) * 240);

				for(j = 0; j < 1 +players; j++) {
					if(background == 2) {
						BlendExBltRect(24, (120 + 192 * j - 96 * players) + tmp1, 40 + tmp2, 0, 0, 80, 160, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
					}

					// グラデーション部分をなめらかに#1.60c7p9ex
					if(!top_frame){
						BlendExBltRect(3, (112 + 192 * j - 96 * players) + tmp1, tmp2, 101, 128, 100 - 20, 33, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
						for(k = 0; k < 20; k++) {
							tr = (256 - fldtr) * (19 - k) / 20;
							tr = 256 - tr;
							BlendExBltRect(3, (112 + 192 * j - 96 * players) + (80 + k) + tmp1, tmp2, 101 + (80 + k), 128, 1, 33, 256 - tr, 256 - tr, 256 - tr, tr, tr, tr);
						}
					}
					else
					{
						BlendExBltRect(3, (97 + 192 * j - 96 * players) + tmp1, tmp2, 127, 423, 126, 35, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
					}
				}
			}

			SwapToSecondary(i);
		}
	}
	EnableBlendColorKey(3, 0);
}

/* 効果音読み込み */
// initializeから独立 #1.60c7o5
void loadWaves(void) {
	/* 効果音を読み込み */
	LoadWave("res/se/shaki.wav", 0);
	LoadWave("res/se/kon.wav", 1);
	LoadWave("res/se/gon.wav", 2);
	LoadWave("res/se/kachi.wav", 3);
	LoadWave("res/se/rotate.wav", 4);
	LoadWave("res/se/move.wav", 5);
	LoadWave("res/se/hold.wav", 6);
	LoadWave("res/se/tumagari.wav", 7);
	LoadWave("res/se/gameover.wav", 8);
	LoadWave("res/se/lvstop.wav", 9);
	LoadWave("res/se/kettei.wav", 10);

	LoadWave("res/se/erase1.wav", 11);
	LoadWave("res/se/erase2.wav", 12);
	LoadWave("res/se/erase3.wav", 13);
	LoadWave("res/se/erase4.wav", 14);

	LoadWave("res/se/ready.wav", 15);
	LoadWave("res/se/go.wav", 16);

	LoadWave("res/se/applause.wav", 17);
	LoadWave("res/se/cheer.wav", 18);
	LoadWave("res/se/levelup.wav", 19);

	LoadWave("res/se/up.wav", 20);
	LoadWave("res/se/block1.wav", 21);
	LoadWave("res/se/block2.wav", 22);
	LoadWave("res/se/block3.wav", 23);
	LoadWave("res/se/block4.wav", 24);
	LoadWave("res/se/block5.wav", 25);
	LoadWave("res/se/block6.wav", 26);
	LoadWave("res/se/block7.wav", 27);

	LoadWave("res/se/ttclear.wav", 28);
	LoadWave("res/se/gm.wav", 29);
	LoadWave("res/se/rankup.wav", 30);
	LoadWave("res/se/stageclear.wav", 31);
	LoadWave("res/se/hurryup.wav", 32);
	LoadWave("res/se/timeover.wav", 33);
	LoadWave("res/se/tspin.wav", 34);
	LoadWave("res/se/hanabi.wav", 35);
	LoadWave("res/se/missionclr.wav", 36);

	LoadWave("res/se/thunder.wav", 37);

	LoadWave("res/se/warning.wav", 38);

	LoadWave("res/se/medal.wav", 39);
	LoadWave("res/se/pinch.wav", 40);

	LoadWave("res/se/platinaerase.wav",41);
	LoadWave("res/se/timeextend.wav",42);
	LoadWave("res/se/stgstar.wav",43);
	LoadWave("res/se/ace_sonic_lock.wav",44);
	LoadWave("res/se/regret.wav",45);
	LoadWave("res/se/cool.wav",46);

	LoadWave("res/se/timestop.wav",47);
	LoadWave("res/se/tserase.wav",48);
	//SetLoopModeWave(40, 1);	//#1.60c7l6
}

/* BGM読み込み */
void loadBGM(void) {
	int i;

	StrCpy(string[0],  "res/bgm/bgm01");		// bgmlv 0 プレイ中（MASTER   0～499）playwave(50)
	StrCpy(string[1],  "res/bgm/bgm02");		// bgmlv 1 プレイ中（MASTER 500～899）
	StrCpy(string[2],  "res/bgm/bgm03");		// bgmlv 2 プレイ中（MASTER 900～998、DEVIL 0～499）
	StrCpy(string[3],  "res/bgm/bgm04");		// bgmlv 3 プレイ中（DEVIL  500～699）
	StrCpy(string[4],  "res/bgm/bgm05");		// bgmlv 4 プレイ中（DEVIL  700～999）
	StrCpy(string[5],  "res/bgm/bgm06");		// bgmlv 5 プレイ中（DEVIL  1000以降）
	StrCpy(string[6],  "res/bgm/ending");		// bgmlv 6 プレイ中（エンディング）
	StrCpy(string[7],  "res/bgm/ending_b");		// bgmlv 7 プレイ中（BEGINNERエンディング）
	StrCpy(string[8],  "res/bgm/tomoyo");		// bgmlv 8 プレイ中 通常（TOMOYO）
	StrCpy(string[9],  "res/bgm/tomoyo_ex");	// bgmlv 9 プレイ中 EXステージ（TOMOYO）
	StrCpy(string[10], "res/bgm/vsmode");		// bgmlv 10 プレイ中（対戦モード）playwave(60)
	StrCpy(string[11], "res/bgm/title");		// bgmlv 11 タイトル
	StrCpy(string[12], "res/bgm/select");		// bgmlv 12 モードセレクト62
	StrCpy(string[13], "res/bgm/nameentry");	// bgmlv 13 ネームエントリー
	StrCpy(string[14], "res/bgm/tomoyo_eh");	// bgmlv 14 プレイ中 E-Heart（TOMOYO）
	StrCpy(string[15], "res/bgm/fever");		// bgmlv 15 FEVER発動中
	StrCpy(string[16], "res/bgm/mission_ex01");	// bgmlv 16 プレイ中 ミッションその1
	StrCpy(string[17], "res/bgm/mission_ex02");	// bgmlv 17 プレイ中 ミッションその2
	StrCpy(string[18], "res/bgm/mission_ex03");	// bgmlv 18 プレイ中 ミッションその3
	StrCpy(string[19], "res/bgm/tomoyo_eh_final");	// bgmlv 19 プレイ E-Heartラストplaywave(69)

	for(i = 0; i <= 19; i++) {
		if(bgmload[i] == 1){
			// 拡張子を決める
			if(wavebgm == 1) StrCat(string[i], ".mid");				// MIDI
			else if(wavebgm == 3) StrCat(string[i], ".ogg");		// OGG
			else if(wavebgm == 4) StrCat(string[i], ".mp3");		// MP3
			else if(wavebgm == 5) StrCat(string[i], ".mod");		// MOD (.mod)
			else if(wavebgm == 6) StrCat(string[i], ".it");			// MOD (.it)
			else if(wavebgm >= 7) StrCat(string[i], ".xm");			// MOD (.xm)
			else StrCat(string[i], ".wav");							// WAV

			// 読み込み
			LoadWave(string[i], 50 + i);

			// ループON
			SetLoopModeWave(50 + i, 1);
		}
	}

	// エンディング曲ループか
	//SetLoopModeWave(56, 0);
	//SetLoopModeWave(57, 0);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  設定内容をバックアップする
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void backupSetups() {
	setupBak[0]  = smooth;
	setupBak[1]  = nanameallow;
	setupBak[2]  = sonicdrop;
	setupBak[3]  = fastlrmove;
	setupBak[4]  = rots[0] +rots[1] * 10;	// 回転方式設定 #1.60c
	setupBak[5]  = item_interval;			// アイテム出現間隔
	setupBak[6]  = w_reverse;				// ワールド時回転方向逆転 #1.60c7f8
	setupBak[7]  = downtype;				// 下入れ#1.60c7f9
	setupBak[8]  = lvupbonus;				// レベルアップボーナス #1.60c7g3
	setupBak[9]  = disable_hold;			// HOLD禁止 #1.60c7n7
	setupBak[10] = disable_irs;				// IRS禁止 #1.60c7n7
	setupBak[11] = disable_wallkick;		// 壁蹴り禁止 #1.60c7q2ex
	setupBak[12] = r_irs;					// IRS基準 #1.60c7q6
	setupBak[13] = world_i_rot;				// ワールドルールのI型の回転法則 #1.60c7r7
	setupBak[14] = spawn_y_type;			// 出現位置タイプ #1.60c7s2
	setupBak[15] = tspin_type;				// T-スピンの種類（DS式でボーナスが追加されたため）C7T6.4
	setupBak[16] = b2bcheck;				// Back to Back
	setupBak[17] = nanamedown;
	setupBak[18] = ace_irs;
	setupBak[19] = ready_go_style;
	setupBak[20] = devil_randrise;			// DEVILのランダムせり上がり
	setupBak[21] = ybigtype;				// BIGの縦移動単位
	setupBak[22] = vs_time;
	setupBak[23] = wintype;
	setupBak[24] = vs_goal;
	setupBak[25] = noitem;
	setupBak[26] = disrise;

}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  設定内容をバックアップから戻す
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void restoreSetups() {
	smooth        = setupBak[0];
	nanameallow   = setupBak[1];
	sonicdrop     = setupBak[2];
	fastlrmove    = setupBak[3];
	rots[0]       = setupBak[4] % 10; 	// 回転方式設定1p #1.60c5
	rots[1]       = setupBak[4] / 10; 	// 2p
	item_interval = setupBak[5];		// アイテム出現間隔
	w_reverse     = setupBak[6];		// ワールド時回転方向逆転 #1.60c7f8
	downtype      = setupBak[7];		// 下入れタイプ #1.60c7f9
	lvupbonus     = setupBak[8];		// レベルアップボーナス #1.60c7g3
	disable_hold  = setupBak[9];		// HOLD禁止 #1.60c7n7
	disable_irs   = setupBak[10];		// IRS禁止 #1.60c7n7
	disable_wallkick = setupBak[11];	// 壁蹴り禁止 #1.60c7q2ex
	r_irs         = setupBak[12];		// IRS基準 #1.60c7q6
	world_i_rot   = setupBak[13];		// ワールドルールのI型の回転法則 #1.60c7r7
	spawn_y_type  = setupBak[14];		// 出現位置タイプ #1.60c7s2
	tspin_type    = setupBak[15];		// T-スピンの種類（DS式でボーナスが追加されたため）C7T6.4
	b2bcheck      = setupBak[16];		// b2b
	nanamedown    = setupBak[17];
	ace_irs       = setupBak[18];
	ready_go_style= setupBak[19];
	devil_randrise= setupBak[20];		// DEVILのランダムせり上がり
	ybigtype      = setupBak[21];		// BIGの縦移動単位
	vs_time       = setupBak[22];
	wintype       = setupBak[23];
	vs_goal       = setupBak[24];
	noitem        = setupBak[25];
	disrise       = setupBak[26];
	use_item[0]   = 0;
	use_item[1]   = 0;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  キーボード入力関連
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
#if		0
int getMaxKey()
{
	return 256;
}

int IsPushDeleteKey()
{
	return IsPushKey(0xD3);
}

int IsPushBSKey()
{
	return IsPushKey(0x0E);
}
#endif

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  halt;
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void spriteTime() {
#if		0
	int ReSize, i, j;

	// 鏡像処理(ツイン台のみ)
	if(mirror && domirror && (maxPlay > 0) && !tomoyo_domirror[0] && !tomoyo_domirror[1]) {
		SwapToSecondary(23);
		// 逆方向に複写
		// ygs2kのExBltRectでは、負方向指定による反転は出来ないらしいのでforループを使用
		if(getDrawRate() == 1){
			for(i = 160; i < 320; i++) {
				BltFastRect(23, 319 - i, 0, i - 160, 0, 1, 240);
			}
		} else {
			for(i = 320; i < 640; i++) {
				BltFastRect(23, 639 - i, 0, i - 320, 0, 1, 480);
			}
		}
		SwapToSecondary(23);
		ExBltFast(23, 160, 0);	// 鏡像を右半分に反映
	}

	if(IsPushKey(ssKey)) {			// スナップショット #1.60c
		sprintf(string[0], "ss\\ss_%04d.png", ssc);
		if( screenMode < 4 ) {
			SwapToSecondary(9);
			SaveBitmap(string[0], 9, 0, 0, sw, sh);
			SwapToSecondary(9);
		} else {
			SaveBitmap(string[0], 9, 0, 0, sw, sh);
		}
		ssc++;
	}

	if(oncap && !(count % capi)) {	// 連続スナップ
		sprintf(string[0], "cap\\hc%06d.png", capc);
		SwapToSecondary(9);
		SaveBitmap(string[0], 9, capx, capy, capw, caph);
		SwapToSecondary(9);
		capc++;
	}
	if(IsPushKey(capKey)) oncap = !oncap;

	if( screenMode >= 4 ) {
		if(screenMode == 4) { ReSize = 25; }
		else if(screenMode == 5) { ReSize = 32; }
		else if(screenMode == 6) { ReSize = 40; }
		else { ReSize = 10; }
		SwapToSecondary(9);		// セカンダリプレーンを、ダミーでプレーン9と入れ替える
		ExBltFastRectR(9, 0, 0, 0, 0, 320, 240, (65536 * ReSize) / 10 / getDrawRate(), (65536 * ReSize) / 10 / getDrawRate());
		SwapToSecondary(9);
		ExBltFast(9, 0, 0);
	}

	// ESCキーが押されたら即刻終了
	if(IsPushEscKey()) {
		//RankingSave();
		//ST_RankingSave();
		maxPlay = tmp_maxPlay;
		if(playback){
			restoreSetups();
			if(!tmp_maxPlay)
				maxPlay = 0;
		}
		SaveConfig();
		Quit();
	}

	halt;

	PlayAllSE();

	ClearSecondary();
#endif

	if ( !YGS2kHalt() )
	{
		maxPlay = tmp_maxPlay;
		if(playback){
			restoreSetups();
			if(!tmp_maxPlay)
				maxPlay = 0;
		}
		SaveConfig();
		loopFlag = false;
	}

	PlayAllSE();
}
