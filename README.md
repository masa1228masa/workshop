# workshop
fingerprint
##登録
"enroll"と打つと登録が始まる。
##モーターの直接操作
"open"と打つとservo.write(180)となる。
"close"と打つとservo.write(0)となる。
##認証
"detect"と打つと認証。この場合"quit"と打たないと終了しない。
servoが"close"状態の時に指紋認証を成功させると"open"状態に移行する。
servoが"open"状態の時に指紋認証をする場合、"close"から"open"状態に移行した時に認証した指紋でしか"close"することができない。

