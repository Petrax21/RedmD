//---------------------------------------------------------------------------

#ifndef PoiskFormUH
#define PoiskFormUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSearchForm : public TForm
{
__published:
        TLabel *Label1;
        TBevel *Bevel2;
        TRadioGroup *RadioGroup;
        TBitBtn *GoBt;
        TBitBtn *CancelBT;
        TEdit *EditIN;
        void __fastcall GoBtClick(TObject *Sender);
        void __fastcall CancelBTClick(TObject *Sender);
private:	// User
public:		// User
        __fastcall TSearchForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSearchForm *SearchForm;
//---------------------------------------------------------------------------
#endif
