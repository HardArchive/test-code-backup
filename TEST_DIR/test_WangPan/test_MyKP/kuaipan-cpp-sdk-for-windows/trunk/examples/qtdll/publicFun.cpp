/*
 * kpSDK是一个基于金山快盘和Qt的自由sdk库，您可以自由分发、修改其中的源代码或者重新发布它，
 * 新的任何修改后的重新发布版必须同样在遵守LGPL3或更后续的版本协议下发布.
 * 关于LGPL协议的细则请参考COPYING、COPYING.LESSER文件，
 * 您可以在kpSDK\examples\qtdll的相关目录中获得LGPL协议的副本，
 * 如果没有找到，请连接到 http://www.gnu.org/licenses/ 查看。
*/
//kuaipan cpp sdk for windows
//Email: michael.20@qq.com
//License: GNU Lesser General Public License (LGPL)
//版权所有 (C) 2012 Michael Lee

#include"publicFun.h"

//hmacSha1算法实现
QString hmacSha1(QByteArray key, QByteArray baseString)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(baseString);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
    //return  hashed.toBase64();//原版的不返回url编码的

    QString signatureStr=hashed.toBase64(); //这里返回url编码
    return QUrl::toPercentEncoding(signatureStr);
}


//返回随机数 oauth_nonce
QString getRandNonce()
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return QString().number(qrand());
}

//返回当前时间
QString getTimeStamp()
{
    time_t curTime=time(0);
    return QString().number(curTime);
}

//去除url编码
QString removeUrlEncode(const QString &urlStr)
{
    QUrl url;
    url.setEncodedUrl(urlStr.toAscii());
    return url.toString();
}


QString stringToUnicode(QString str)
{
    // 这里传来的字符串一定要加tr，main函数里可以加 QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //  例如：str=tr("你好");
    const QChar *q;
    QChar qtmp;
    QString str0, strout;
    int num;
    q=str.unicode();
    int len=str.count();
    for(int i=0;i<len;i++)
    {   qtmp =(QChar)*q++;
        num= qtmp.unicode();
        if(num<255)
            strout+="00"; //英文或数字前加"00"

        str0=str0.setNum(num,16);//变成十六进制数

        strout+=str0;
    }
    return strout;
}

QString unicodeToString(QString str)
{
    //例如 str="4F60597D";
    int temp[400];
    QChar qchar[100];
    QString strOut;
    bool ok;
    int count=str.count();
    int len=count/4;
    for(int i=0;i<count;i+=4)
    {
     temp[i]=str.mid(i,4).toInt(&ok,16);//每四位转化为16进制整型
     qchar[i/4]=temp[i];
     QString str0(qchar, len);
     strOut=str0;
    }
    return strOut;
}

QString s2q(const string &s)
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}

string q2s(const QString &s)
{
    return string((const char *)s.toLocal8Bit());
}
