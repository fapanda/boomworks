/**
 * @file CppCounter.cpp
 * @brief implementation of the CppCounter class.
 * @author Boom( boomworks@hotmail.com )
 * @author Copyright(C) 2004-2005 BoomWorks.Net , All right reserved.
 * @date 2005-12-10
 * $Revision: $
 */

#include <wx/textfile.h>
#include "CppCounter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CppCounter
//////////////////////////////////////////////////////////////////////
CppCounter* CppCounter::ms_instance = 0;

CppCounter::CppCounter()
{}

CppCounter::~CppCounter()
{}

CppCounter* CppCounter::GetInstance()
{
    if ( ms_instance == 0)
    {
        ms_instance = new CppCounter;
    }
    return ms_instance;
}

void CppCounter::countingSourceFile(wxTextFile& file, int& nSize, int& nLines, int& nCodeLines, int& nCommentLines, int& nBlankLines )
{
    // get file size
    // nSize = file.Len();

    //////////////////////////////////////////////////////////////////////////

    BOOL bCommentSet = FALSE; //AΛΞRΔ΄ o"/*"―TRUE, "*/"―FALSE
    BOOL bQuatoSet = FALSE;   //u¬RΔ΄ ΥΜΉΞoδϋτ"―TRUE, aΉΞoδϋτ"―FALSE

    wxString bufRead;


    for ( bufRead = file.GetFirstLine(); !file.Eof(); bufRead = file.GetNextLine() )
    {
        BOOL bStatedComment = FALSE;//]Ξ¨AΛΞΕΟRΔ
        BOOL bStatedCode = FALSE;   //]Ξ¨ψιΞΕΟRΔ

        nLines++;

        bufRead.Trim(false); //gJΒόVctοΔ£

        if ( bufRead.Len() ==0 ) //¨tΥΞ
        {
            nBlankLines++;
            continue;
        }

        if ( bCommentSet && bufRead.Find( _T("*/") ) == -1 )
        {
            nCommentLines++;
            continue;
        }

        if ( bufRead.Find( _T("//") ) ==-1 && bufRead.Find( _T("/*") ) ==-1 && bufRead.Find( _T("*/") ) ==-1 )
        {//ε]Ξχ]ΛάAΛEπ¨ΉΕAΛE¨ΉΕψιΞ
            if ( bCommentSet )
            {
                nCommentLines++;
                continue;
            }
            else
            {
                if ( bufRead.Find( _T('"') ) ==-1 )
                {
                    nCodeLines++;
                    continue;
                }
            }
        }

        if ( bufRead.Find( _T("//") ) ==0 && !bCommentSet && !bQuatoSet )
        {
            nCommentLines++;
            continue;
        }

        BOOL bDoubleSplashFound = FALSE;
        BOOL bSplashStarFound = FALSE;

        for (unsigned int i=0; i<bufRead.Len()-1; i++)
        {
            // TCHAR cTemp = bufRead[i];
            if ( bufRead[i] == _T('/') && bufRead[i+1] == _T('/') && !bCommentSet && !bQuatoSet )
            {
                if ( !bStatedComment
                        && ( m_nLineCountingType == NStatisticAll || m_nLineCountingType == NStatisticCommentOnly ))
                {
                    bStatedComment = TRUE;
                    nCommentLines++;
                }
                bDoubleSplashFound = TRUE;
                //i++;//EΑ+1EJ°Zo°Sb€
                break;
            }
            else if ( bufRead[i] == _T('/') && bufRead[i+1] == _T('*') && !bCommentSet && !bQuatoSet )
            {
                if ( !bStatedComment
                        && ( m_nLineCountingType == NStatisticAll || m_nLineCountingType == NStatisticCommentOnly ))
                {
                    bStatedComment = TRUE;
                    nCommentLines++;
                }
                bCommentSet = TRUE;
                bSplashStarFound = TRUE;
                i++;
            }
            //ΔψιΞwιΨbCommentSetwt¬O
            else if ( bufRead[i] != _T(' ') && bufRead[i] != _T('\t') && !bCommentSet )
            {
                if ( !bStatedCode )
                {
                    bStatedCode = TRUE;
                    nCodeLines++;
                }
                if ( bufRead[i] == _T('\\') )
                {//\¬ρcu¨φ
                    i++;
                    continue;
                }
                if ( bufRead[i]=='\'' )
                {
                    if ( bufRead[i+1]=='\\' )
                        i+=2;
                    else
                        i+=1;
                    continue;
                }
                if ( bufRead[i]=='"' )
                {//"wιϋξΦΡEΞΉltzhou
                    bQuatoSet = !bQuatoSet;
                }
            }
            else if ( bufRead[i]=='*' && bufRead[i+1]=='/' && bCommentSet && !bQuatoSet )
            {
                if ( !bStatedComment
                        && ( m_nLineCountingType == NStatisticAll || m_nLineCountingType == NStatisticCommentOnly))
                {
                    bStatedComment = TRUE;
                    nCommentLines++;
                }
                bCommentSet = FALSE;
                bSplashStarFound = TRUE;
                i++;
            }
        }

        if ( bDoubleSplashFound )
        {
            if ( m_nLineCountingType == NStatisticCommentOnly && bStatedCode ) //εRΔ\G¨yϋΤEqK―oψιΞAΛΞEπΉΔAΛΞ
            {
                nCodeLines--;
            }

            if ( m_nLineCountingType == NStatisticCodeOnly && !bStatedCode ) //εRΔ\G¨yΉΤEq²¨ψιΞRΔEΕSw¨AΛΞ
            {
                nCommentLines++;
            }
            continue;
        }

        if ( bufRead[bufRead.Len() -1]=='"'&&!bCommentSet )
        {//ς±ΞρΉτΕ"Eπw¦b²wtbQuatoSetEΕψιΞΉΞEπGλ
            bQuatoSet = !bQuatoSet;
            if ( !bQuatoSet )
            {
                if ( !bStatedCode )
                {
                    bStatedCode = TRUE;
                    nCodeLines++;
                }
            }
            else
            {
//				CStdioFile fileLog;
//				if( fileLog.Open( m_strLogFile, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate ) ==TRUE )
//				{
//					wxString strMsg;
//					if( fileLog.Len() ==0 )
//					{
//						strMsg.Format( "Βό\tΞ\tΘΰ\n", strFileName, nLines );
//						fileLog.WriteString( strMsg );
//					}
//					strMsg.Format( "%s\t%d\tu¬ZΞ²b\\\n", strFileName, nLines );
//					fileLog.WriteString( strMsg );
//					fileLog.Close();
//				}
            }
            continue;
        }

        if ( bufRead[bufRead.Len() -1]!=' ' && bufRead[bufRead.Len() -1]!='\t' && !bCommentSet
                && bufRead[bufRead.Len() -2]!='*' && bufRead[bufRead.Len() -1]!='/' )
        {//ερΉτuftοΔEqOά/*Eρ\τuΉΕ*/Eπ¨ψιΞ
            if ( !bStatedCode )
            {
                bStatedCode = TRUE;
                nCodeLines++;
            }
        }

        if ( bSplashStarFound )
        {
            if ( m_nLineCountingType == NStatisticCommentOnly && bStatedCode ) //εRΔ\G¨yϋΤEqK―oψιΞAΛΞEπΉΔAΛΞ
            {
                nCodeLines--;
            }

            if ( m_nLineCountingType == NStatisticCodeOnly && !bStatedCode && !bStatedComment ) 	//ςΑΞάψιε    /*abc*/ //222
                //JΕRΔ\GΕyΉΤEπζ¨VΡAΛΞΔϋΉΜ
            {
                bStatedComment = TRUE;
                nCommentLines++;
            }
        }

        if ( !bStatedComment && bCommentSet ) //hΪΕOo/*EΨyΉΤRΔ\GΞE²¨ψιΞΔEΕS]Ξn¦ΕAΛΞ
        {
            if ( m_nLineCountingType == NStatisticCodeOnly && !bStatedCode )
            {
                bStatedComment = TRUE;
                nCommentLines++;
            }
        }
    }
}
