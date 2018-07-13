# tetlisDirectxVer.0.0.1
[HumanAcademy OsakaCampus] TETRIS created by DirectX9 C Language
# TETRIMINER
![tetriminer_logo](https://user-images.githubusercontent.com/39621839/42644829-79c84bac-8637-11e8-95df-722ac8bc614f.png)
### マイニングアクションパズル
## ゲーム説明
**時間によってせりあがってくるブロックをアイテムやラインをそろえて消していこう
基本ルールは~~TETRIS~~と同じだが同時に多数のラインを消すと追加で消えるラインが増えるぞ!!**
## 操作方法
|キー|動作|
|---|---|
|R |回転|
|SPACE|ホールド|
|下左右左右矢印|下左右移動|
|上矢印|ハードドロップ|
|Vを押しながらのQとR|アイテムの選択|
|NUM1|アイテムの使用位置選択状態へ移行|
|アイテムの使用位置選択状態時の上下左右矢印|アイテムの使用場所移動|
|アイテムの使用位置選択状態時のENTER|アイテムの使用|
|BACKSPACE|リセット|
|ESCAPE|ゲーム終了|
## ブロック
|ブロック|説明|
|---|---|
|**耐久値ブロック**|**mライン消しによるm分の耐久値減少<br>を用いることによって壊すことができるブロック**|
|耐久値1ブロック|![dur1block](https://user-images.githubusercontent.com/39621839/42642932-e337ef5c-8632-11e8-850c-3f21aec8e716.png)|
|耐久値2ブロック|![dur2block](https://user-images.githubusercontent.com/39621839/42642949-ecbad9ae-8632-11e8-823d-0aabcb81322e.png)|
|耐久値3ブロック|![dur3block](https://user-images.githubusercontent.com/39621839/42642963-f5b87bb0-8632-11e8-8885-bc2d2b7d213b.png)|
|||
|**アイテムブロック**|**消すことによってブロックに応じた<br>アイテムが獲得できる耐久値１のブロック**<br>***狙撃銃以外のアイテムでは取得できない***|
|ウルトラドリルブロック|![ultradrillblock](https://user-images.githubusercontent.com/39621839/42642663-4f496dc0-8632-11e8-9789-f8c8598c809a.png)|
|レーザーカノンブロック|![lasercannonblock](https://user-images.githubusercontent.com/39621839/42643070-3a865dd4-8633-11e8-91d9-083378627c76.png)|
|狙撃銃ブロック|![sniperblock](https://user-images.githubusercontent.com/39621839/42642725-710823b6-8632-11e8-95cd-e85e16961c8e.png)|
|エクスカリバーブロック|![excaliberblock](https://user-images.githubusercontent.com/39621839/42642750-83611a68-8632-11e8-83cf-7cdcf588c1b5.png)|
|||
|**テトリミノブロック**|**上から降ってくる耐久値１のブロック**|
|I型のテトリミノブロック|![redblock](https://user-images.githubusercontent.com/39621839/42641445-82c2a52a-862f-11e8-9869-d3fe9fe1fbd7.png)|
|J型テトリミノブロック|![orangeblock](https://user-images.githubusercontent.com/39621839/42641548-b9ef7d3e-862f-11e8-97bc-48d06fc54bf9.png)|
|L型テトリミノブロック|![yellowblock](https://user-images.githubusercontent.com/39621839/42641579-ccfb789c-862f-11e8-83cb-74a4a1fd890b.png)|
|S型テトリミノブロック|![greenblock](https://user-images.githubusercontent.com/39621839/42641631-e6ba4af6-862f-11e8-8700-4bd98ea7c363.png)|
|Z型テトリミノブロック|![waterblueblock](https://user-images.githubusercontent.com/39621839/42641653-f3c745f0-862f-11e8-9c62-7b7b1657dc35.png)|
|T型テトリミノブロック|![blueblock](https://user-images.githubusercontent.com/39621839/42641723-27f04480-8630-11e8-9dc4-859c28c6a2f6.png)|
|O型テトリミノブロック|![violetblock](https://user-images.githubusercontent.com/39621839/42641749-3443da80-8630-11e8-843d-5effe7fb56b2.png)|
|||
|**銃弾ブロック**|狙撃銃を使用したときに放たれる耐久値1のブロック|
||![shotbulletblock](https://user-images.githubusercontent.com/39621839/42643036-26cbc91e-8633-11e8-8bf7-aaec9825d2d7.png)|
## アイテム
|アイテム名|効果|
|---|---|
|ウルトラドリル|全てのブロックを破壊する|
|レーザーカノン|縦3横10のブロックを破壊する|
|狙撃銃|ブロックを埋めその場所に配置されていたアイテムを取得する|
|エクスカリバー|縦20横３のブロックを破壊する|
***
ヒューマンアカデミー大阪校で合同製作中の作品です


**著者**
*辻野 晴崇*

**2018/07/12**
