//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "PoiskInteger.h"
#include "Windows.h"
#include "ProcessInfoU.h"
#include "MainFormU.h"
long PoiskInteger (DWORD ProcessHandle,AnsiString BPoiskString,  LPVOID VirtualMem){
      SYSTEM_INFO              Sinfo;
      MEMORY_BASIC_INFORMATION MemInfo;
      long                     Start = 0, Fin = 0, Param = 0, FPage = 0, FindKol = 0, SPage = 0, WriteK = 0;
      unsigned char            MasPoisk[4095], PoiskB;
      HANDLE                   OpenHandle = NULL;
      LPVOID                   Point = NULL;
      DWORD                    MemRead, *FileWrite;
      TVarRec                  Item;

PoiskB = (unsigned char) BPoiskString.ToInt();
SPage  =  PageKol(ProcessHandle);
GetSystemInfo(&Sinfo);
FileWrite  = reinterpret_cast<LPDWORD> ( VirtualAlloc(VirtualMem, 4096, MEM_COMMIT, PAGE_READWRITE) );
OpenHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessHandle);
//----------------------Searching--------------------------------------
do{
   VirtualQueryEx(OpenHandle, Point, &MemInfo, sizeof(MemInfo));
     if ((MemInfo.Protect==PAGE_READWRITE) && (MemInfo.State!=MEM_FREE)){
        InterlockedIncrement(&FPage);
        Start = reinterpret_cast<DWORD>(MemInfo.BaseAddress);
        Fin = Start + MemInfo.RegionSize;
     do{
         ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(Start), &MasPoisk, 4096, &MemRead);
         for (Param=0;Param<=4095;InterlockedIncrement(&Param))
             if (MasPoisk[Param]==PoiskB){
                 FileWrite[WriteK] = Start+Param;
                 InterlockedIncrement(&FindKol);
                 InterlockedIncrement(&WriteK);
                 if (WriteK == 1024){
                    FileWrite = reinterpret_cast<LPDWORD> ( VirtualAlloc( reinterpret_cast<LPVOID>(reinterpret_cast<DWORD>(FileWrite)+ 4096),  4096, MEM_COMMIT, PAGE_READWRITE) );
                    WriteK = 0;}
                 if (MainForm->AddressTable->Count<=300){
                    Item =reinterpret_cast<LPVOID>(Start+Param);
                    MainForm->AddressTable->Items->Add(Format(": %p", &Item, 0));}
             }
         Application->ProcessMessages();
         InterlockedExchangeAdd(&Start,4096);
        }while (Start!=Fin);
  }Point = reinterpret_cast<LPVOID>(MemInfo.RegionSize + reinterpret_cast<DWORD>(Point));
}while (FPage!=SPage);
//-----------------------------Close handles------------------------------
CloseHandle(OpenHandle);
return FindKol;
};
//*****************************************************************************
//*****************************************************************************
long  PoiskIntegerAgainQucly (DWORD ProcessHandle,AnsiString BPoiskString,  LPVOID VirtualMem, long FindNumber){
      unsigned char            PoiskAll = 0, PoiskB;
      long                     FindKol = 0, WriteK = 0, ReadK = 0, Pages = 0;
      DWORD                    MemRead = 0, *FileWrite, *FileRead;
      HANDLE                   OpenHandle = NULL;
      TVarRec                  Item; 

PoiskB     = (unsigned char) BPoiskString.ToInt();
FileWrite  = reinterpret_cast<LPDWORD> (VirtualMem);
FileRead   = reinterpret_cast<LPDWORD> (VirtualMem);
OpenHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessHandle);
//----------------------Searching again-----------------------------------
for(FindKol=0;FindKol<FindNumber;InterlockedIncrement(&FindKol)){
       ReadProcessMemory(OpenHandle , reinterpret_cast<LPVOID>(FileRead[ReadK]), &PoiskAll, 1, &MemRead);
       if (PoiskAll==PoiskB){
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
//-----------------------------Close handles------------------------------
CloseHandle(OpenHandle);
return    Pages*1024+WriteK;
};
//************************************************************************
//---------------------------------------------------------------------------

#pragma package(smart_init)
 