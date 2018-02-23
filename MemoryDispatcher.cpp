#pragma once
#include "stdafx.h"

namespace MemoryDispatcher
{
    const string INPUT_TEXT_FILE_NAME = "Karamzin_page.txt";
    const string INPUT_DATA_FILE_NAME = "page1.dat";
    const string OUTPUT_TEXT_FILE_NAME = "result.txt.";

    const int PAGE_SIZE = 256;
    const int VIRTUAL_PAGES_COUNT = 256;
    const int PHYSICAL_PAGES_COUNT = 128;

    class Frame
    {
    public:
        int index;
        string content;

        Frame(){}

        Frame(int frameIndex, string frameContent) {
            this->index = frameIndex;
            this->content = frameContent;
        };

        char getSymbol(int offset) {
            try {
                return this->content[offset];
            }
            catch (...) {
                printf("ERROR ON REFERENSING TO PAGE\n");
                return ' ';
            }
        };

    };

    class InputFileReader
    {
    public:
        InputFileReader() {
            this->readFile();
            this->createFrames();
        }

        Frame getFrame(int frameNumber) {
            return this->frames[frameNumber];
        }
    private:
        void readFile() {
            ifstream file(INPUT_TEXT_FILE_NAME);
            string currentString;
            while (getline(file, currentString)) {
                fileContent += currentString;
            }
            file.close();
        }
        void createFrames() {
            for (int index = 0; index < VIRTUAL_PAGES_COUNT; index++) {
                int startSymbolNumber = PAGE_SIZE * index;
                int endSymbolNumber = startSymbolNumber + PAGE_SIZE-1;
                string frameContent = fileContent.substr(startSymbolNumber, endSymbolNumber);
                Frame frame = *(new Frame(index, frameContent));
                this->frames[index] = frame;
            }
        }
        Frame frames[VIRTUAL_PAGES_COUNT];
        string fileContent;
    };

    struct TLB_Frame
    {
        int frameIndex;
        int physicalFrameIndex;
        bool isLoaded;
    };

    class TLB
    {
    public:
        array<TLB_Frame, VIRTUAL_PAGES_COUNT> frames;

        TLB() {
            for (int index = 0; index < VIRTUAL_PAGES_COUNT; index++) {
                TLB_Frame frame = *(new TLB_Frame());
                frame.frameIndex = index;
                frame.isLoaded = false;
                frame.physicalFrameIndex = -1;

                this->frames[index] = frame;
            }
        }

        TLB_Frame getVirtualFrame(int filePageNumber) {
            return frames.at(filePageNumber);
        };

        void setPhysicalFrameStatus(int physicalFrameIndex, bool status) {
            for (int index = 0; index < VIRTUAL_PAGES_COUNT; index++) {
                TLB_Frame frame = frames[index];
                if (frame.physicalFrameIndex == physicalFrameIndex) {
                    frame.isLoaded = status;
                    if (status == false) {
                        frame.physicalFrameIndex = -1;
                    }
                    break;
                }
            }
        }        
    };

    class PhysicalMemory
    {
    public:
        deque<Frame> frames;

        Frame getFrame(int frameNumber) {
            for (Frame page : this->frames) {
                if (page.index = frameNumber) { return page; }
            }
        }
    };

    class MemoryDispatcher
    {
    public:
        MemoryDispatcher() {
            this->fileReader = *(new InputFileReader());
            this->tlb = *(new TLB());
            this->physicalMemory = *(new PhysicalMemory());
        };

        void run() {
            ifstream file(INPUT_DATA_FILE_NAME, ios::binary);
            ofstream out(OUTPUT_TEXT_FILE_NAME);
            int data = 0;
            while (!file.eof()) {
                file.read((char*)&data, sizeof(data));
                int offset = data & 255;
                int pageNumber = (data >> 8) & 255;

                TLB_Frame tlbFrame = this->tlb.getVirtualFrame(pageNumber);
                if (!tlbFrame.isLoaded) {
                    this->loadPage(pageNumber);
                }
                tlbFrame = this->tlb.getVirtualFrame(pageNumber);
                Frame frame = this->physicalMemory.frames.at(tlbFrame.physicalFrameIndex);
                char symbol = frame.getSymbol(offset);
                out << symbol;

                cout << pageNumber << " " << offset << " " << symbol << endl;
            }
            out.close();
            file.close();
        }
    private:
        InputFileReader fileReader;
        TLB tlb;
        PhysicalMemory physicalMemory;

        void loadPage(int pageNumber) {
            this->physicalMemory.frames.push_front(this->fileReader.getFrame(pageNumber));
            this->tlb.setPhysicalFrameStatus(pageNumber, true);
            this->tlb.frames.at(pageNumber).frameIndex = this->physicalMemory.frames.size() - 1;

            if (this->physicalMemory.frames.size() > PHYSICAL_PAGES_COUNT) {
                Frame deletedPage = this->physicalMemory.frames.back();
                this->tlb.setPhysicalFrameStatus(deletedPage.index, false);
                this->physicalMemory.frames.pop_back();
            }

            this->sync();
        }

        void sync() {
            for (int index = 0; index < this->physicalMemory.frames.size(); index++) {
                Frame page = this->physicalMemory.frames.at(index);
                this->tlb.frames.at(page.index).physicalFrameIndex = index;
            }
        }
    };
}