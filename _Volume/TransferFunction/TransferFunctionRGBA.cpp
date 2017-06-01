// StereoViewer is a volume rendering program with a stereoscopic view
// Copyright (C) 2009 Pascal Mues
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "_Volume/TransferFunction/TransferFunctionRGBA.h"

TransferFunctionRGBA::TransferFunctionRGBA() : iTransferFunction()
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::TransferFunctionRGBA) Executing", FuncUp);
    m_uiTexName = NULL;

    m_bColorNodeClear();
    m_bAlphaNodeClear();

    m_bGenerateColorMap();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::TransferFunctionRGBA) OK", FuncDown);
}
TransferFunctionRGBA::~TransferFunctionRGBA()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::~TransferFunctionRGBA) Executing", FuncUp);
    if(m_uiTexName) glDeleteTextures(1, &m_uiTexName);
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::~TransferFunctionRGBA) OK", FuncDown);
}
bool TransferFunctionRGBA::m_bColorNodeAdd(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bColorNodeAdd) Executing");
    if(m_aColorNodes[R].m_iFindIndexToGray(gray) >= 0)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bColorNodeAdd) ColorNode schon vorhanden! Umgeleitet zu this->m_bColorNodeChange");
        return this->m_bColorNodeChange(gray, red, green, blue);
    }
    int indexr = m_aColorNodes[R].m_iAddNodeSorted(gray, red),
        indexg = m_aColorNodes[G].m_iAddNodeSorted(gray, green),
        indexb = m_aColorNodes[B].m_iAddNodeSorted(gray, blue);
    if((indexr == indexg) && (indexr == indexb) && (indexg == indexb))
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bColorNodeAdd) Node hinzugefügt(Index=" + QString::number(indexr) + " Gray=" +
                                    QString::number(gray) + " Red=" + QString::number(red) + " Green=" + QString::number(green) + " Blue="+ QString::number(blue) + ")");
        return m_bGenerateColorMap();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bColorNodeAdd) Indizes stimmen nicht überein! Tranferfunktion fehlerhaft! indexr=" + QString::number(indexr) +
                                    "indexg=" + QString::number(indexg) +
                                    "indexb=" + QString::number(indexb) +
                                    "m_aColorNodes[R].m_Nodes.count()=" + QString::number(m_aColorNodes[R].m_Nodes.count()) +
                                    "m_aColorNodes[G].m_Nodes.count()=" + QString::number(m_aColorNodes[G].m_Nodes.count()) +
                                    "m_aColorNodes[B].m_Nodes.count()=" + QString::number(m_aColorNodes[B].m_Nodes.count()));
        return false;
    }
}
bool TransferFunctionRGBA::m_bColorNodeChange(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue)
{
    // geht davon aus das die indizes in allein listen gleich sind!!!
    // evtl. gefährlich wenn dies nicht der fall ist!!!
    int index = m_aColorNodes[0].m_iFindIndexToGray(gray);
    if (index < 0)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bColorNodeChange) ColorNode nicht gefunden. gray=" + QString::number(gray));
        return false;
    }
    bool br = m_aColorNodes[R].m_bChangeNode(gray, red),
         bg = m_aColorNodes[G].m_bChangeNode(gray, green),
         bb = m_aColorNodes[B].m_bChangeNode(gray, blue);
    if (br && bg && bb)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bColorNodeChange) Node hinzugefügt(Gray=" +
                                    QString::number(gray) + " Red=" + QString::number(red) + " Green=" + QString::number(green) + " Blue="+ QString::number(blue));
        return m_bGenerateColorMap();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bColorNodeChange) gray=" + QString::number(gray) +
                                    "red=" + QString::number(red) +
                                    "green=" + QString::number(green) +
                                    "blue=" + QString::number(blue) +
                                    "index=" + QString::number(index) +
                                    "br=" + QString::number(br) +
                                    "bg=" + QString::number(bg) +
                                    "bb=" + QString::number(bb));
        return false;
    }
}
bool TransferFunctionRGBA::m_bColorNodeRemove(unsigned char gray)
{
    if(gray==0||gray==255) return false;

    // geht davon aus das die indizes in allein listen gleich sind!!!
    // evtl. gefährlich wenn dies nicht der fall ist!!!
    int index = m_aColorNodes[0].m_iFindIndexToGray(gray);
    if (index < 0)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bColorNodeRemove) ColorNode nicht gefunden. gray=" + QString::number(gray));
        return false;
    }
    bool br = m_aColorNodes[R].m_bRemoveNode(index), bg = m_aColorNodes[G].m_bRemoveNode(index), bb = m_aColorNodes[B].m_bRemoveNode(index);
    if (br && bg && bb)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bColorNodeRemove) ColorNode removed. gray=" + QString::number(gray));
        return m_bGenerateColorMap();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bColorNodeRemove) gray=" + QString::number(gray) +
                                    "index=" + QString::number(index) +
                                    "br=" + QString::number(br) +
                                    "bg=" + QString::number(bg) +
                                    "bb=" + QString::number(bb));
        return false;
    }
}
bool TransferFunctionRGBA::m_bColorNodeClear()
{
    // Clears the Colornode list and sets Node 0 and 255 as default!
    m_aColorNodes[R].m_Nodes.clear();
    m_aColorNodes[G].m_Nodes.clear();
    m_aColorNodes[B].m_Nodes.clear();

    m_aColorNodes[R].m_vAddNode(0,0);
    m_aColorNodes[G].m_vAddNode(0,0);
    m_aColorNodes[B].m_vAddNode(0,0);

    m_aColorNodes[R].m_vAddNode(255,0);
    m_aColorNodes[G].m_vAddNode(255,0);
    m_aColorNodes[B].m_vAddNode(255,0);

    m_aColorNodes[R].m_vSortNodes();
    m_aColorNodes[G].m_vSortNodes();
    m_aColorNodes[B].m_vSortNodes();
    return true;
}
bool TransferFunctionRGBA::m_bAlphaNodeAdd(unsigned char gray, unsigned char alpha)
{
    if(m_AlphaNodes.m_iFindIndexToGray(gray) >= 0)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bAlphaNodeAdd) AlphaNode schon vorhanden! Umgeleitet zu this->m_bAlphaNodeChange");
        return this->m_bAlphaNodeChange(gray, alpha);
    }
    int indexa = m_AlphaNodes.m_iAddNodeSorted(gray, alpha);
    if(indexa)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bAlphaNodeAdd) AlphaNode hinzugefügt(Index=" + QString::number(indexa) + " Gray=" +
                                    QString::number(gray) + " Alpha=" + QString::number(alpha) + ")");
        return m_bGenerateColorMap();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bAlphaNodeAdd) Indizes stimmen nicht überein! Tranferfunktion fehlerhaft! m_AlphaNodes.m_Nodes.count()=" + QString::number(m_AlphaNodes.m_Nodes.count()));
        return false;
    }
}
bool TransferFunctionRGBA::m_bAlphaNodeChange(unsigned char gray, unsigned char alpha)
{
    int index = m_AlphaNodes.m_iFindIndexToGray(gray);
    if (index < 0)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bAlphaNodeChange) AlphaNode nicht gefunden. gray=" + QString::number(gray));
        return false;
    }
    bool ba = m_AlphaNodes.m_bChangeNode(gray, alpha);
    if (ba)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bAlphaNodeChange) AlphaNode changed(Gray=" +
                                    QString::number(gray) + " Alpha=" + QString::number(alpha));
        return m_bGenerateColorMap();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bAlphaNodeChange) gray=" + QString::number(gray) +
                                    "alpha=" + QString::number(alpha) +
                                    "index=" + QString::number(index) +
                                    "ba=" + QString::number(ba));
        return false;
    }
}
bool TransferFunctionRGBA::m_bAlphaNodeRemove(unsigned char gray)
{
    if(gray==0||gray==255) return false;
    int index = m_AlphaNodes.m_iFindIndexToGray(gray);
    if(index < 0)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bAlphaNodeRemove) AlphaNode nicht gefunden. gray=" + QString::number(gray));
        return false;
    }
    if(m_AlphaNodes.m_bRemoveNode(index))
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bAlphaNodeRemove) AlphaNode removed. gray=" + QString::number(gray));
        return m_bGenerateColorMap();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bAlphaNodeRemove) gray=" + QString::number(gray) +
                                    "index=" + QString::number(index));
        return false;
    }
}
bool TransferFunctionRGBA::m_bAlphaNodeClear()
{
    m_AlphaNodes.m_Nodes.clear();
    m_AlphaNodes.m_vAddNode(0, 255);
    m_AlphaNodes.m_vAddNode(255, 255);
    m_AlphaNodes.m_vSortNodes();
    return true;
}
const NodesList* TransferFunctionRGBA::m_GetAllColorNodes() const
{
    return m_aColorNodes;
}
const NodesList TransferFunctionRGBA::m_GetAllAlphaNodes() const
{
    return m_AlphaNodes;
}
const unsigned char TransferFunctionRGBA::m_ucGetRedToGray(unsigned char gray)
{
    return m_aColorMap[gray*m_ciSizeOfColor+0];
}
const unsigned char TransferFunctionRGBA::m_ucGetGreenToGray(unsigned char gray)
{
    return m_aColorMap[gray*m_ciSizeOfColor+1];
}
const unsigned char TransferFunctionRGBA::m_ucGetBlueToGray(unsigned char gray)
{
    return m_aColorMap[gray*m_ciSizeOfColor+2];
}
const unsigned char TransferFunctionRGBA::m_ucGetAlphaToGray(unsigned char gray)
{
    return m_aColorMap[gray*m_ciSizeOfColor+3];
}

bool TransferFunctionRGBA::m_bBuildColorTableTexture()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bBuildColorTableTexture) Executing", FuncUp);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bBuildColorTableTexture) Got glError on entering this function (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bBuildColorTableTexture) No glErrors on entering function");

    // gets a new texture name for the colortable, this usually used in the unified params for shaders
    if(m_uiTexName != NULL)
    {
        glDeleteTextures(1, &m_uiTexName);
        glGenTextures(1, &m_uiTexName);
    }
    else
        glGenTextures(1, &m_uiTexName);

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bBuildColorTableTexture) glGenTextures() got glError (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bBuildColorTableTexture) No glErrors on glGenTextures()");

    // rebind the texture data to the texture name we got at init
    glBindTexture(GL_TEXTURE_1D, m_uiTexName);
    /*glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);*/

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bBuildColorTableTexture) Got glError on setting Texture-Parameters (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bBuildColorTableTexture) No glErrors setting Texture-Parameters");

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, m_ciArraySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_aColorMap);

    //if(m_LogFile->m_eLogMode==Debug) this->m_vSaveAsImage("ColorTable.png");
    this->m_vSaveAsImage("ColorTable.png");
    this->m_vSaveAsFile("TransferfunctionDebug.txt");

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_bBuildColorTableTexture) Got glError on glTexImage1D(..) (Error-Code " + QString::number(err) + ")", FuncDown);
        return false;
    }
    else
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bBuildColorTableTexture) BuildColorTableTexture .. OK", FuncDown);
        return true;
    }
}
const unsigned int TransferFunctionRGBA::m_uiGetTextureName() const
{
    return m_uiTexName;
}

bool TransferFunctionRGBA::m_bGenerateColorMap()
{
    // Diese Funktion generiert die ColorMap. Das heisst es sollen lineare Farb/Alpha-Verläufe
    // von einem Farb/Alpha-Node generiert werden und in m_aColorMap abgelegt werden.
    // Die indizierung von m_aColorMap ist wie folgt:
    //
    // m_aColorMap[0]=Rot für Grauwert 0
    // m_aColorMap[1]=Grün für Grauwert 0
    // m_aColorMap[2]=Blau für Grauwert 0
    // m_aColorMap[3]=Alpha für Grauwert 0
    //
    // m_aColorMap[4]=Rot für Grauwert 1
    // m_aColorMap[5]=Grün für Grauwert 1
    // m_aColorMap[6]=Blau für Grauwert 1
    // m_aColorMap[7]=Alpha für Grauwert 1
    //
    // und so weiter.
    //
    // Aus dieser Indizierung kann direkt eine 1D Textur mit dem Format GL_RGBA erstellt werden.

    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bGenerateColorMap) Executing");
    int CountR = m_aColorNodes[R].m_Nodes.count(),
        CountG = m_aColorNodes[R].m_Nodes.count(),
        CountB = m_aColorNodes[R].m_Nodes.count(),
        CountA = m_AlphaNodes.m_Nodes.count();

    // Prüfen ob mindestens 2 Knoten in jeder Liste sind!
    if(CountR < 2 || CountG < 2 || CountB < 2 || CountA < 2)
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_vGenerateColorMap) Eine Nodesliste hat weniger wie 2 Knoten!");
        return false;
    }

    // Prüfen ob die Anzahlen der Knoten in allen ColorListen übereinstimmen
    if(CountR != CountG || CountR != CountB || CountG != CountB)
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_vGenerateColorMap) ColorNode Listen haben unterschiedliche Counts! CountR=" + QString::number(CountR) +
                                    "CountG=" + QString::number(CountG) +
                                    "CountB=" + QString::number(CountB));
        return false;
    }

    // Durchgehen aller Nodes in den ColorListen und Berechung eines linearen Farbverlaufes zwischen ihnen
    for(int index = 0; index < CountR; index++)
    {
        double VonGrayR, VonDataR, BisGrayR, BisDataR;
        double VonGrayG, VonDataG, BisGrayG, BisDataG;
        double VonGrayB, VonDataB, BisGrayB, BisDataB;
        double DataStepSizeR, DataStepSizeG, DataStepSizeB;

        VonGrayR = m_aColorNodes[R].m_Nodes[index].Gray;
        BisGrayR = m_aColorNodes[R].m_Nodes[index+1].Gray;
        VonDataR = m_aColorNodes[R].m_Nodes[index].Data;
        BisDataR = m_aColorNodes[R].m_Nodes[index+1].Data;

        VonGrayG = m_aColorNodes[G].m_Nodes[index].Gray;
        BisGrayG = m_aColorNodes[G].m_Nodes[index+1].Gray;
        VonDataG = m_aColorNodes[G].m_Nodes[index].Data;
        BisDataG = m_aColorNodes[G].m_Nodes[index+1].Data;

        VonGrayB = m_aColorNodes[B].m_Nodes[index].Gray;
        BisGrayB = m_aColorNodes[B].m_Nodes[index+1].Gray;
        VonDataB = m_aColorNodes[B].m_Nodes[index].Data;
        BisDataB = m_aColorNodes[B].m_Nodes[index+1].Data;

        // Werte der Knoten Prüfen:
        // Alle Grauwerte "Von" müssen gleich sein und alle Grauwerte "Bis" müssen gleich sein!
        if(VonGrayR != VonGrayG || VonGrayR != VonGrayB || VonGrayG != VonGrayB)
        {
            m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_vGenerateColorMap) VonGray's ungleich! VonGrayR=" + QString::number(VonGrayR) +
                                        "VonGrayG=" + QString::number(VonGrayG) +
                                        "VonGrayB=" + QString::number(VonGrayB));
            return false;
        }
        if(BisGrayR != BisGrayG || BisGrayR != BisGrayB || BisGrayG != BisGrayB)
        {
            m_LogFile->m_bAppendMessage(Error, "(TransferFunctionRGBA::m_vGenerateColorMap) BisGray's ungleich! BisGrayR=" + QString::number(BisGrayR) +
                                        "BisGrayG=" + QString::number(BisGrayG) +
                                        "BisGrayB=" + QString::number(BisGrayB));
            return false;
        }

        // Berechnen des Farbwert-Inkrements das mit einer Graustufenerhöhung erfolgt
        // Vorzeichen ergibt sich hierdurch automatisch!
        // z.B.: von R=200 nach R=250 auf 10 Schritten -> RInkrement=  5
        //       von R=250 nach R=200 auf 10 Schritten -> RInkrement= -5 !!!
        DataStepSizeR = (BisDataR-VonDataR) / (BisGrayR-VonGrayR);
        DataStepSizeG = (BisDataG-VonDataG) / (BisGrayG-VonGrayG);
        DataStepSizeB = (BisDataB-VonDataB) / (BisGrayB-VonGrayB);

        for(int pos = VonGrayR; pos <= BisGrayR; pos++)
        {
            unsigned char ValR, ValG, ValB;
            ValR = VonDataR + DataStepSizeR*(pos - VonGrayR);
            ValG = VonDataG + DataStepSizeG*(pos - VonGrayG);
            ValB = VonDataB + DataStepSizeB*(pos - VonGrayB);
            m_aColorMap[pos*m_ciSizeOfColor+0] = ValR;
            m_aColorMap[pos*m_ciSizeOfColor+1] = ValG;
            m_aColorMap[pos*m_ciSizeOfColor+2] = ValB;
        }

        // Wenn letzter Bis zum 255er Knoten gezeichent, for-Schleife verlassen!
        // da die BisGray's ja gleich sind reicht es einen zu prüfen
        if(BisGrayR == 255) break;
    }

    // Durchgehen aller Nodes in der AlphaListe und Berechung eines linearen Transparenzverlaufes
    for(int index = 0; index < CountA; index++)
    {
        double VonGrayA, VonDataA, BisGrayA, BisDataA;
        double DataStepSizeA;

        VonGrayA = m_AlphaNodes.m_Nodes[index].Gray;
        BisGrayA = m_AlphaNodes.m_Nodes[index+1].Gray;
        VonDataA = m_AlphaNodes.m_Nodes[index].Data;
        BisDataA = m_AlphaNodes.m_Nodes[index+1].Data;

        // Berechnen des Alphawert-Inkrements das mit einer Graustufenerhöhung erfolgt
        // Vorzeichen ergibt sich hierdurch automatisch!
        // z.B.: von A=200 nach A=250 auf 10 Schritten -> AInkrement=  5
        //       von A=250 nach A=200 auf 10 Schritten -> AInkrement= -5 !!!
        DataStepSizeA = (BisDataA-VonDataA) / (BisGrayA-VonGrayA);

        for(int pos = VonGrayA; pos <= BisGrayA; pos++)
        {
            unsigned char ValA;
            ValA = VonDataA + DataStepSizeA*(pos - VonGrayA);
            m_aColorMap[pos*m_ciSizeOfColor+3] = ValA;
        }

        // Wenn letzter Bis zum 255er Knoten gezeichent, for-Schleife verlassen!
        // da die BisGray's ja gleich sind reicht es einen zu prüfen
        if(BisGrayA == 255) break;
    }

    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionRGBA::m_bGenerateColorMap) OK");
    return true;
}

void TransferFunctionRGBA::m_vSaveAsImage(QString filename)
{
    int hoehe = 10;
    QImage img = QImage(256, hoehe, QImage::Format_ARGB32);
    for(int i=0; i<m_ciArraySize; i++)
    {
        QRgb x = qRgba(m_aColorMap[i*m_ciSizeOfColor+0],
                      m_aColorMap[i*m_ciSizeOfColor+1],
                      m_aColorMap[i*m_ciSizeOfColor+2],
                      m_aColorMap[i*m_ciSizeOfColor+3]);
        for(int j=0; j<hoehe; j++)
        {
            img.setPixel(i, j, x);
        }
    }
    img.save(filename,"PNG", 100);
}

void TransferFunctionRGBA::m_vSaveAsFile(QString filename)
{
    QStringList TF;
    TF.append("Gray\tR\tG\tB\tA");
    for(int i=0; i<m_ciArraySize; i++)
    {
        TF.append(QString::number(i) + "\t" +
                  QString::number(m_aColorMap[i*m_ciSizeOfColor+0]) + "\t" +
                  QString::number(m_aColorMap[i*m_ciSizeOfColor+1]) + "\t" +
                  QString::number(m_aColorMap[i*m_ciSizeOfColor+2]) + "\t" +
                  QString::number(m_aColorMap[i*m_ciSizeOfColor+3]));
    }

    QFile MyFile(filename);
    if (MyFile.open (QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream stream( &MyFile );
        for(QStringList::Iterator it = TF.begin(); it != TF.end(); ++it)
        {
            stream << *it << EndLine;
        }
        MyFile.close();
    }
}
