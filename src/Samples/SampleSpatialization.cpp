

// For starting the WTF library
#include <wtf/ExportMacros.h>
#include "MainThread.h"

// webaudio specific headers
#include "AudioBufferSourceNode.h"
#include "AudioContext.h"
#include "BiquadFilterNode.h"
#include "ConvolverNode.h"
#include "ExceptionCode.h"
#include "GainNode.h"
#include "MediaStream.h"
#include "MediaStreamAudioSourceNode.h"
#include "OscillatorNode.h"
#include "PannerNode.h"

// LabSound
#include "RecorderNode.h"
#include "SoundBuffer.h"

#include "LiveEcho.h"

#include <unistd.h>
#include <iostream>

using namespace WebCore;
using LabSound::RecorderNode;
using LabSound::SoundBuffer;

void sampleSpatialization(RefPtr<AudioContext> context, float seconds)
{
    ExceptionCode ec;
    
    //--------------------------------------------------------------
    // Demonstrate 3d spatialization and doppler shift
    //
    SoundBuffer train(context, "trainrolling.wav");
    RefPtr<PannerNode> panner = context->createPanner();
    panner->connect(context->destination(), 0, 0, ec);
    PassRefPtr<AudioBufferSourceNode> trainNode = train.play(panner.get(), 0.0f);
    trainNode->setLooping(true);
    context->listener()->setPosition(0, 0, 0);
    panner->setVelocity(15, 0, 0);
    float halfTime = seconds * 0.5f;
    for (float i = 0; i < seconds; i += 0.01f) {
        float x = (i - halfTime) / halfTime;
        // keep it a bit up and in front, because if it goes right through the listener
        // at (0, 0, 0) it abruptly switches from left to right.
        panner->setPosition(x, 0.1f, 0.1f);
        usleep(10000);
    }
}
