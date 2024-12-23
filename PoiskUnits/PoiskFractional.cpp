//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "PoiskFractional.h"
#include "Windows.h"
#include "ProcessInfoU.h"
#include "MainFormU.h"
//-----------------------------------
union FractionalFloat {
      unsigned char     BArray[4];
               float    BFloat;
}PoiskFloat;

union FractionalDouble {
      unsigned char     BArray[8];
               double   BDouble;
}PoiskDouble;

union FractionalLDouble {
      unsigned char     BArray[10];
        long double     BLDouble;
}PoiskLDouble;
//-----------------------------------
boolean _STRCHK(char *S1, char *S2, long length){
for(long i=0;i<length;InterlockedIncrement(&i))
    if (S1[i]!=S2[i])return FALSE;
return TRUE;}
//-----------------------------------
long PoiskFractional(DWORD ProcessHandle,AnsiString BPoiskString, LPVOID VirtualMem){

      SYSTEM_INFO              Sinfo;
      MEMORY_BASIC_INFORMATION MemInfo;
      long                     Start = 0, Fin = 0, Param = 0, FPage = 0, FindKol = 0, SPage = 0, WriteK = 0;
      unsigned char            MasPoisk[4095], BF[4], BD[8], BLD[10];
      HANDLE                   OpenHandle = NULL;
      LPVOID                   Point = NULL;
      DWORD                    *FileWrite, MemRead;
      TVarRec                  Item;
//-------------------------------------------------------------------
      PoiskFloat.BFloat     = BPoiskString.ToDouble();
      PoiskDouble.BDouble   = BPoiskString.ToDouble();
      PoiskLDouble.BLDouble = BPoiskString.ToDouble();
//---------------------------------------------------------
OpenHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessHandle);
SPage      = PageKol(ProcessHandle);
FileWrite  = reinterpret_cast<LPDWORD> ( VirtualAlloc(VirtualMem, 4096, MEM_COMMIT, PAGE_READWRITE) );
GetSystemInfo(&Sinfo);
//-----------------------------------------------------------------
do{
   VirtualQueryEx(OpenHandle, Point, &MemInfo, sizeof(MemInfo));
     if ((MemInfo.Protect==PAGE_READWRITE) && (MemInfo.State!=MEM_FREE)){
          InterlockedIncrement(&FPage);
          Start = reinterpret_cast<DWORD>(MemInfo.BaseAddress);
          Fin = Start + MemInfo.RegionSize;
     do {
          ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(Start), &MasPoisk, 4096, &MemRead);
          for (Param=0;Param<=4095;InterlockedIncrement(&Param))
              if ( (MasPoisk[Param] == PoiskFloat.BArray[0]) || (MasPoisk[Param] == PoiskDouble.BArray[0]) || (MasPoisk[Param] == PoiskLDouble.BArray[0]) ){
                 ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(Start+Param), BF, 4,   &MemRead);
                 ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(Start+Param), BD, 8,   &MemRead);
                 ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(Start+Param), BLD,10,  &MemRead);
                  if ( _STRCHK(PoiskFloat.BArray,BF,4)==TRUE || _STRCHK(PoiskDouble.BArray,BD,8)==TRUE || _STRCHK(PoiskLDouble.BArray, BLD,10)==TRUE ){
                       FileWrite[WriteK] = Start+Param;
                       InterlockedIncrement(&FindKol);
                       InterlockedIncrement(&WriteK);
                       if (WriteK == 1024){
                          FileWrite = reinterpret_cast<LPDWORD> ( VirtualAlloc( reinterpret_cast<LPVOID>(reinterpret_cast<DWORD>(FileWrite)+ 4096),  4096, MEM_COMMIT, PAGE_READWRITE) );
                          WriteK = 0;}
                       if (MainForm->AddressTable->Count<=500){
                          Item =reinterpret_cast<LPVOID>(Start+Param);
                          MainForm->AddressTable->Items->Add(Format(": %p", &Item, 0));}
                     }
                  }
          Application->ProcessMessages();
          InterlockedExchangeAdd(&Start,4096);
        }while (Start!=Fin);
     }Point = reinterpret_cast<LPVOID>(MemInfo.RegionSize + reinterpret_cast<DWORD>(Point));
}while (FPage!=SPage);
CloseHandle(OpenHandle);
return FindKol;
}
//---------------------------------------------------------------------------
//***************************************************************************
long PoiskFractionalAgainQucly(DWORD ProcessHandle,AnsiString BPoiskString,  LPVOID VirtualMem,long FindNumber){

   unsigned char            PoiskAll, BF[4], BD[8], BLD[10];
   long                     FindKol = 0, WriteK = 0, ReadK = 0, Pages = 0;
   DWORD                    MemRead = 0, *FileWrite, *FileRead;
   HANDLE                   OpenHandle = NULL;
   TVarRec                  Item;
//-------------------------------------------------------------------
   PoiskFloat.BFloat     = BPoiskString.ToDouble();
   PoiskDouble.BDouble   = BPoiskString.ToDouble();
   PoiskLDouble.BLDouble = BPoiskString.ToDouble();
   OpenHandle            = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessHandle);
//------------------------------------------------------------
FileWrite  = reinterpret_cast<LPDWORD> (VirtualMem);
FileRead   = reinterpret_cast<LPDWORD> (VirtualMem);
//-----------------------------------------------------------------------------
for(FindKol=0;FindKol<FindNumber;InterlockedIncrement(&FindKol)){
       ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(FileRead[ReadK]), BF, 4, &MemRead);
       ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(FileRead[ReadK]), BD, 8, &MemRead);
       ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(FileRead[ReadK]), BLD,10,&MemRead);
       if ( _STRCHK(PoiskFloat.BArray,BF,4)==TRUE || _STRCHK(PoiskDouble.BArray,BD,8)==TRUE || _STRCHK(PoiskLDouble.BArray, BLD,10)==TRUE ){
          FileWrite[WriteK] = FileRead[ReadK];
          InterlockedIncrement(&WriteK);
                if (MainForm->AddressTable->Count<=500){
                   Item =reinterpret_cast<LPVOID>(FileWrite[ReadK]);
                   MainForm->AddressTable->Items->Add(Format(": %p", &Item, 0));}
                if (WriteK == 1024){
                   FileWrite = reinterpret_cast<LPDWORD> (reinterpret_cast<DWORD>(FileWrite)+ 4096);
                   InterlockedIncrement(&Pages);
                   WriteK = 0;}
          }
       InterlockedIncrement(&ReadK);
       if (ReadK  == 1024){
                 FileRead = reinterpret_cast<LPDWORD> ( reinterpret_cast<DWORD>(FileRead)+ 4096 );
                    ReadK = 0;}
      Application->ProcessMessages();
   }
//-----------------------------------------------------------------------------   
CloseHandle(OpenHandle);
return    Pages*1024+WriteK;
}      
//---------------------------------------------------------------------------

#pragma package(smart_init)
 