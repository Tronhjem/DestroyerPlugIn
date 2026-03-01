# Destroyer – Copilot Instructions

## Project Overview
Destroyer is a JUCE-based audio distortion VST/VST3 plugin. Distortion is implemented as a piecewise-linear lookup table driven by a draggable translation graph in the UI, followed by a 4-pole Moog ladder lowpass filter.

## Build

The project is managed by **Projucer** (`Destroyer.jucer`). Native IDE project files are pre-generated under `Builds/`.

| Platform | Build |
|----------|-------|
| macOS | Open `Builds/MacOSX/Destroyer.xcodeproj` in Xcode |
| Windows | Open `Builds/VisualStudio2019/Destroyer.sln` in VS 2019 |

After editing `Destroyer.jucer`, re-save in Projucer to regenerate IDE projects before building.

JUCE modules are resolved from a sibling path: `../../../JUCE/modules` (Mac) or `../../juce` (Windows). Make sure JUCE lives at the expected relative location.

There is no CMake, no test suite, and no lint step.

## Architecture

### Signal flow
```
Audio input → TranslationCurve::GetYValue() → MoogFilter::Process() → Audio output
```

- **`PluginProcessor`** owns `juce::AudioProcessorValueTreeState apvts` (10 `AudioParameterFloat` params), a `TranslationCurve curve`, and a `MoogFilter[2]` array (one per channel). Raw parameter pointers are cached at construction and read in `processBlock`.
- At the start of each `processBlock`, all 10 raw atomics are read to sync `curve.points` and the filter call — this keeps DAW automation live without any listener overhead.
- Processing is gated by `if (editor != nullptr)`.
- **`PluginEditor`** owns the `Envelope` (curve editor widget) and `SineWaveVisual` (preview widget). On editor creation the processor's `curve` and `apvts` pointers are wired into `envelope`. Sliders are connected to `apvts` via `SliderAttachment` (no manual `Slider::Listener`).

### Parameter layout (`createParameterLayout()`)

| ID   | Name              | Range       | Default | Notes                      |
|------|-------------------|-------------|---------|----------------------------|
| freq | Frequency         | 20–20000 Hz | 19000   | Skew factor 0.2            |
| res  | Resonance         | 0.0–1.0     | 0.2     |                            |
| pt0y | Curve Point 0 Y   | -1.0–1.0    | -1.0    | X permanently locked to -1 |
| pt1x | Curve Point 1 X   | -1.0–1.0    | -0.5    |                            |
| pt1y | Curve Point 1 Y   | -1.0–1.0    | -0.5    |                            |
| pt2x | Curve Point 2 X   | -1.0–1.0    | 0.0     |                            |
| pt2y | Curve Point 2 Y   | -1.0–1.0    | 0.0     |                            |
| pt3x | Curve Point 3 X   | -1.0–1.0    | 0.5     |                            |
| pt3y | Curve Point 3 Y   | -1.0–1.0    | 0.5     |                            |
| pt4y | Curve Point 4 Y   | -1.0–1.0    | 1.0     | X permanently locked to +1 |

All 10 parameters are DAW-automatable. State persistence uses APVTS XML serialization in `getStateInformation`/`setStateInformation`.

### Distortion curve system (`Envelope.hpp`)
- `TranslationCurve` holds a `std::vector<EnvelopePoint>` sorted by X. All coordinate values are normalized to **[-1, 1]** (matching audio sample range).
- `GetYValue(x)` performs linear interpolation between the two surrounding points.
- `EnvelopePoint` supports `PointRestrict` flags (`LockX`, `LockY`, `LockXY`) — the two outer points are X-locked so the curve always covers the full input range.
- The `Envelope` JUCE component handles mouse interaction (drag to move points, double-click to reset). After each change it calls `notifyApvts(idx)` to push the new position into APVTS via `setValueNotifyingHost`, enabling DAW automation recording.

### Moog filter (`MoogFilter`)
- 4-pole ladder filter implementation. **Sample rate is hardcoded** as `static constexpr double SAMPLERATE = 1.0 / 44100.0` — it is not updated from `prepareToPlay`. Be aware of this when working with filter behaviour at non-44100 Hz rates.
- One `MoogFilter` instance per channel to maintain independent state.

### UI (`PluginEditor`)
- Fixed window size: **900 × 600** (`WIDTH`/`HEIGHT` in `PluginEditor.h`).
- Color palette defined as macros at the top of `PluginEditor.h`: `DEEP_PURPLE`, `LIGHT_PURPLE`, `PINK`, `TURQOISE`.
- Two separate `LookAndFeel_V4` subclasses (`DestroyerLookAndFeelFREQ`, `DestroyerLookAndFeelRES`) apply the same visual style to the two sliders.

## Key Conventions

- **State persistence** — `getStateInformation`/`setStateInformation` serialize the full APVTS state as XML binary. All 10 parameters are saved and recalled automatically.
- **No oversampling or anti-aliasing** — the distortion is applied directly at the host sample rate.
- Header guards: non-JUCE files use `#ifndef` include guards; `PluginProcessor.h` / `PluginEditor.h` use `#pragma once`.
- Targets VST and VST3 formats only (`buildVST,buildVST3`); `JUCE_VST3_CAN_REPLACE_VST2=0`.
