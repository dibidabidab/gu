
#include "audio.h"

au::Sound::~Sound()
{
    alCall(alDeleteBuffers, 1, &buffer);
}

au::SoundSource::SoundSource(const au::Sound &sound)
{
    alCall(alGenSources, 1, &source);
    alCall(alSourcei, source, AL_BUFFER, sound.buffer);
}

au::SoundSource::~SoundSource()
{
    alCall(alDeleteSources, 1, &source);
}

bool au::SoundSource::isPlaying()
{
    ALint state;
    alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void au::SoundSource::stop()
{
    alCall(alSourceStop, source);
}

void au::SoundSource::play()
{
    alCall(alSourcePlay, source);
}

void au::SoundSource::setPitch(float p)
{
    alCall(alSourcef, source, AL_PITCH, p);
}

void au::SoundSource::setVolume(float g)
{
    alCall(alSourcef, source, AL_MAX_GAIN, g);
    alCall(alSourcef, source, AL_GAIN, g);
}

void au::SoundSource::setPosition(const vec3 &v)
{
    alCall(alSource3f, source, AL_POSITION, v.x, v.y, v.z);
}

void au::SoundSource::setVelocity(const vec3 &v)
{
    alCall(alSource3f, source, AL_VELOCITY, v.x, v.y, v.z);
}

void au::SoundSource::setLooping(bool b)
{
    alCall(alSourcei, source, AL_LOOPING, b);
}

void au::SoundSource::pause()
{
    alCall(alSourcePause, source);
}

bool au::SoundSource::hasStopped()
{
    ALint state;
    alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

bool au::SoundSource::isPaused()
{
    ALint state;
    alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}

ALCdevice* au::openALDevice = NULL;
ALCcontext* au::openALContext = NULL;

void au::init()
{
    openALDevice = alcOpenDevice(nullptr);
    if(!openALDevice)
    {
        #ifdef linux
        throw gu_err("No OpenAL device found!\n"
                     "This is a known issue that might happen when you only have OSS installed, which is deprecated.\n"
                     "Try installing ALSA (libasound2-dev) and/or Pulseaudio (libpulse-dev pulseaudio apulse).");
        #else
        throw gu_err("No OpenAL device found!");
        #endif

        // todo: add support for running game without audio.
    }


    if(!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext)
        throw gu_err("Could not create audio context!");

    ALCboolean contextMadeCurrent = false;
    if(!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext) || contextMadeCurrent != ALC_TRUE)
        throw gu_err("Could not make audio context current!");
}

void au::terminate()
{
    if(!alcCall(alcDestroyContext, openALDevice, openALContext))
        std::cerr << "Couldn't destroy OpenAL context\n";

    ALCboolean closed;
    if(!alcCall(alcCloseDevice, closed, openALDevice, openALDevice))
        std::cerr << "Couldn't close OpenAL device\n";
}

bool au::getAvailableDevices(std::vector<std::string> &devicesVec, ALCdevice *device)
{
    const ALCchar* devices;
    if(!alcCall(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER))
        return false;

    const char* ptr = devices;

    devicesVec.clear();
    do
    {
        devicesVec.push_back(std::string(ptr));
        ptr += devicesVec.back().size() + 1;
    }
    while(*(ptr + 1) != '\0');

    return true;
}

bool au::check_alc_errors(const std::string &filename, std::uint_fast32_t line, ALCdevice *device)
{
    ALCenum error = alcGetError(device);
    if(error != ALC_NO_ERROR)
    {
        std::cerr << "*** OpenAL Error *** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
            case ALC_INVALID_VALUE:
                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case ALC_INVALID_DEVICE:
                std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
                break;
            case ALC_INVALID_CONTEXT:
                std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
                break;
            case ALC_INVALID_ENUM:
                std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
                break;
            case ALC_OUT_OF_MEMORY:
                std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
                break;
            default:
                std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

bool au::check_al_errors(const std::string &filename, std::uint_fast32_t line)
{
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        std::cerr << "*** OpenAL Error *** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
            case AL_INVALID_NAME:
                std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
                break;
            case AL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                break;
            default:
                std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}
