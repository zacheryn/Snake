#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <string>
#include <iostream>

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif


// XAudio2 Audio Engine Interface
class Audio {
private:

	IXAudio2* pXAudio2 = nullptr; //Base pointer to engine
	IXAudio2MasteringVoice* pMasterVoice = nullptr; //Pointer to mastering voice, final destination for all audio
	std::string BasePath;


	//Finds the chunks of a valid audio file pulled directlt from XAudio2 documentation
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());

        DWORD dwChunkType;
        DWORD dwChunkDataSize;
        DWORD dwRIFFDataSize = 0;
        DWORD dwFileType;
        DWORD bytesRead = 0;
        DWORD dwOffset = 0;

        while (hr == S_OK) {
            DWORD dwRead;
            if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());

            if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());

            switch (dwChunkType) {
            case fourccRIFF:
                dwRIFFDataSize = dwChunkDataSize;
                dwChunkDataSize = 4;
                if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                    hr = HRESULT_FROM_WIN32(GetLastError());
                break;

            default:
                if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                    return HRESULT_FROM_WIN32(GetLastError());
            }

            dwOffset += sizeof(DWORD) * 2;

            if (dwChunkType == fourcc) {
                dwChunkSize = dwChunkDataSize;
                dwChunkDataPosition = dwOffset;
                return S_OK;
            }

            dwOffset += dwChunkDataSize;

            if (bytesRead >= dwRIFFDataSize) return S_FALSE;

        }

        return S_OK;
	}


	//Reads the data in a given chunk pulled directly from XAudio2 documentation
    HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());
        DWORD dwRead;
        if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }


public:

	//Initializes XAudio2 as per documentaion upon object creation
	//and also takes in the path to sound files
	Audio(char* path) {
		HRESULT hr;
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr)) std::cout << hr;

		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) std::cout << hr;

		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice))) std::cout << hr;

		BasePath = std::string(path);
	}


	//Initializes XAudio2 as per documentaion upon object creation
	//and also takes in the path to sound files
	Audio(std::string path) {
		HRESULT hr;
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr)) std::cout << hr;

		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) std::cout << hr;

		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice))) std::cout << hr;

		BasePath = path;
	}


	//Takes in the file name of a .wav file, volume level, and whether the sound is to loop
	//And plays the appropiate sounds in the requested fashion
    int Play(std::string filename, float volume, bool ShouldLoop) {
        filename = BasePath + filename;
        HRESULT hr;
        WAVEFORMATEXTENSIBLE wfx = { 0 };
        XAUDIO2_BUFFER buffer = { 0 };

    #ifdef _XBOX
        char* strFileName = filename;
    #else
        TCHAR* strFileName = new TCHAR[filename.size() + 1];
        strFileName[filename.size()] = 0;
        copy(filename.begin(), filename.end(), strFileName);
    #endif

        // Open the file
        HANDLE hFile = CreateFile(
            strFileName,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (INVALID_HANDLE_VALUE == hFile)
            return HRESULT_FROM_WIN32(GetLastError());

        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());

        DWORD dwChunkSize;
        DWORD dwChunkPosition;
        //check the file type, should be fourccWAVE or 'XWMA'
        FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
        DWORD filetype;
        ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
        if (filetype != fourccWAVE)
            return S_FALSE;

        FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
        ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

        //fill out the audio data buffer with the contents of the fourccDATA chunk
        FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
        BYTE* pDataBuffer = new BYTE[dwChunkSize];
        ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

        buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
        buffer.pAudioData = pDataBuffer;  //buffer containing audio data
        buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

        if (ShouldLoop) buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
        IXAudio2SourceVoice* pSourceVoice;
        if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) return hr;
        if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer))) return hr;
        pSourceVoice->SetVolume(volume);
        if (FAILED(hr = pSourceVoice->Start(0))) return hr;

        return 0;
    }
};

#endif