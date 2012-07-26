/*
 * kpSDK��һ�����ڽ�ɽ���̺�Qt������sdk�⣬���������ɷַ����޸����е�Դ����������·�������
 * �µ��κ��޸ĺ�����·��������ͬ��������LGPL3��������İ汾Э���·���.
 * ����LGPLЭ���ϸ����ο�COPYING��COPYING.LESSER�ļ���
 * ��������kpSDK\examples\qtdll�����Ŀ¼�л��LGPLЭ��ĸ�����
 * ���û���ҵ��������ӵ� http://www.gnu.org/licenses/ �鿴��
*/
//kuaipan cpp sdk for windows
//Email: michael.20@qq.com
//License: GNU Lesser General Public License (LGPL)
//��Ȩ���� (C) 2012 Michael Lee

#include"publicFun.h"

//hmacSha1�㷨ʵ��
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
    //return  hashed.toBase64();//ԭ��Ĳ�����url�����

    QString signatureStr=hashed.toBase64(); //���ﷵ��url����
    return QUrl::toPercentEncoding(signatureStr);
}


//��������� oauth_nonce
QString getRandNonce()
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return QString().number(qrand());
}

//���ص�ǰʱ��
QString getTimeStamp()
{
    time_t curTime=time(0);
    return QString().number(curTime);
}

//ȥ��url����
QString removeUrlEncode(const QString &urlStr)
{
    QUrl url;
    url.setEncodedUrl(urlStr.toAscii());
    return url.toString();
}


QString stringToUnicode(QString str)
{
    // ���ﴫ�����ַ���һ��Ҫ��tr��main��������Լ� QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //  ���磺str=tr("���");
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
            strout+="00"; //Ӣ�Ļ�����ǰ��"00"

        str0=str0.setNum(num,16);//���ʮ��������

        strout+=str0;
    }
    return strout;
}

QString unicodeToString(QString str)
{
    //���� str="4F60597D";
    int temp[400];
    QChar qchar[100];
    QString strOut;
    bool ok;
    int count=str.count();
    int len=count/4;
    for(int i=0;i<count;i+=4)
    {
     temp[i]=str.mid(i,4).toInt(&ok,16);//ÿ��λת��Ϊ16��������
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
