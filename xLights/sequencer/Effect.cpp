#include "Effect.h"
#include "EffectLayer.h"
#include "../BitmapCache.h"


class ControlRenameMap {
public:
    ControlRenameMap() {
        data["E_TEXTCTRL_Morph_Start_X1"] = "";
        data["E_TEXTCTRL_Morph_Start_Y1"] = "";
        data["E_TEXTCTRL_Morph_Start_X2"] = "";
        data["E_TEXTCTRL_Morph_Start_Y2"] = "";
        data["E_TEXTCTRL_MorphStartLength"] = "";
        data["E_TEXTCTRL_Morph_End_X1"] = "";
        data["E_TEXTCTRL_Morph_End_Y1"] = "";
        data["E_TEXTCTRL_Morph_End_X2"] = "";
        data["E_TEXTCTRL_Morph_End_Y2"] = "";
        data["E_TEXTCTRL_MorphEndLength"] = "";
        data["E_TEXTCTRL_MorphDuration"] = "";
        data["E_TEXTCTRL_MorphAccel"] = "";
        data["E_TEXTCTRL_Pictures_GifSpeed"] = "";
        data["E_TEXTCTRL_PicturesXC"] = "";
        data["E_TEXTCTRL_PicturesYC"] = "";
        data["E_CHECKBOX_MorphUseHeadStartColor"] = "";
        data["E_CHECKBOX_MorphUseHeadEndColor"] = "";
        data["E_SLIDER_Chase_Spacing1"] = "";
        
        data["E_NOTEBOOK_Text1"] = "";
        data["E_TEXTCTRL_Pictures_Filename"] = "E_FILEPICKER_Pictures_Filename";
        data["E_TEXTCTRL_Text_Font1"] = "E_FONTPICKER_Text_Font1";
        data["E_TEXTCTRL_Text_Font2"] = "E_FONTPICKER_Text_Font2";
        data["E_TEXTCTRL_Text_Font3"] = "E_FONTPICKER_Text_Font3";
        data["E_TEXTCTRL_Text_Font4"] = "E_FONTPICKER_Text_Font4";
    }
    const void map(wxString &n) const {
        wxStringToStringHashMap::const_iterator it = data.find(n);
        if (it != data.end()) {
            n = it->second;
        }
    }
private:
    wxStringToStringHashMap data;
} Remaps;

void SettingsMap::RemapChangedSettingKey(wxString &n,  wxString &value) {
    Remaps.map(n);
}

void AdjustSettingsToBeFitToTime(int effectIdx, SettingsMap &settings, int startMS, int endMS) {
    int ftt = wxAtoi(settings.Get("T_CHECKBOX_FitToTime", "1"));
    switch (effectIdx) {
        //these effects have never used the FitToTime or speed settings, nothing to do
        case BitmapCache::RGB_EFFECTS_e::eff_OFF:
        case BitmapCache::RGB_EFFECTS_e::eff_GALAXY:
        case BitmapCache::RGB_EFFECTS_e::eff_FAN:
        case BitmapCache::RGB_EFFECTS_e::eff_MORPH:
        case BitmapCache::RGB_EFFECTS_e::eff_SPIROGRAPH:
        case BitmapCache::RGB_EFFECTS_e::eff_SHOCKWAVE:
        case BitmapCache::RGB_EFFECTS_e::eff_GLEDIATOR:
        case BitmapCache::RGB_EFFECTS_e::eff_FACES:
            break;
        
        //these effects have been updated to have a dedicated repeat or speed or other control
        //and now ignore the FitToTime and Speed sliders, but the settings need adjusting
        case BitmapCache::RGB_EFFECTS_e::eff_ON:
            if (settings.Get("E_SLIDER_On_Cycles", "") == "") {
                int cycles = 10;
                if (!ftt) {
                    int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                    int totalTime = endMS - startMS;
                    int maxState = totalTime * speed / 50;
                    cycles = maxState / 20.0;
                }
                settings["E_SLIDER_On_Cycles"] = wxString::Format("%d", cycles);
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_BUTTERFLY:
            if (settings.Get("E_SLIDER_Butterfly_Speed", "") == "") {
                //Butterfly never used the FitToTime, just speed
                settings["E_SLIDER_Butterfly_Speed"] = settings.Get("T_SLIDER_Speed", "10");
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_CIRCLES:
            if (settings.Get("E_SLIDER_Circles_Speed", "") == "") {
                //Circles never used the FitToTime, just speed
                settings["E_SLIDER_Circles_Speed"] = settings.Get("T_SLIDER_Speed", "10");
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_LIFE:
            if (settings.Get("E_SLIDER_Life_Speed", "") == "") {
                //Life never used the FitToTime, just speed
                settings["E_SLIDER_Life_Speed"] = settings.Get("T_SLIDER_Speed", "10");
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_COLORWASH:
            if (settings.Get("E_SLIDER_ColorWash_Cycles", "") == "") {
                int count = wxAtoi(settings.Get("E_SLIDER_ColorWash_Count", "1"));
                settings.erase("E_SLIDER_ColorWash_Count");
                if (settings["T_CHECKBOX_FitToTime"] == "1") {
                    count = 1;
                    settings["E_CHECKBOX_ColorWash_CircularPalette"] = "0";
                } else {
                    settings["E_CHECKBOX_ColorWash_CircularPalette"] = "1";
                }
                settings["E_SLIDER_ColorWash_Cycles"] = wxString::Format("%d", count * 10);
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_FIRE:
            if (settings.Get("E_CHECKBOX_Fire_GrowFire", "") != "") {
                bool grow = settings["E_CHECKBOX_Fire_GrowFire"] == "1";
                settings.erase("E_CHECKBOX_Fire_GrowFire");
                if (grow) {
                    int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                    int totalTime = endMS - startMS;
                    double maxState = totalTime * speed / 50;
                    double cycles = maxState / 500.0;
                    int icycles = cycles * 10;
                    settings["E_SLIDER_Fire_GrowthCycles"] = wxString::Format("%d", icycles);
                } else {
                    settings["E_SLIDER_Fire_GrowthCycles"] = "0";
                }
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_FIREWORKS:
            if (settings.Get("E_SLIDER_Fireworks_Number_Explosions", "") != "") {
                int cnt = wxAtoi(settings["E_SLIDER_Fireworks_Number_Explosions"]);
                settings.erase("E_SLIDER_Fireworks_Number_Explosions");
                int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                int total = (speed * cnt) / 50;
                if (total > 50) {
                    total = 50;
                }
                if (total < 1) {
                    total = 1;
                }
                settings["E_SLIDER_Fireworks_Explosions"] = wxString::Format("%d", total);
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_RIPPLE:
            if (settings.Get("E_SLIDER_Ripple_Cycles", "") == "") {
                int cycles = 10;
                if (!ftt) {
                    int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                    int totalTime = endMS - startMS;
                    int maxState = totalTime * speed / 50;
                    cycles = maxState / 20.0;
                }
                settings["E_SLIDER_Ripple_Cycles"] = wxString::Format("%d", cycles);
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_BARS:
            if (settings.Get("E_SLIDER_Bars_Cycles", "") == "") {
                int cycles = 10;
                wxString dir = settings["E_CHOICE_Bars_Direction"];
                if (!ftt) {
                    int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                    int totalTime = endMS - startMS;
                    int maxState = totalTime * speed / 50;
                    if (dir.Contains("Altern")) {
                        cycles = maxState / 5.0;
                    } else {
                        cycles = maxState / 20.0;
                    }
                }
                settings["E_SLIDER_Bars_Cycles"] = wxString::Format("%d", cycles);
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_SPIRALS:
            if (settings.Get("E_SLIDER_Spirals_Movement", "") == "") {
                int cycles = 10;
                int dir = wxAtoi(settings["E_SLIDER_Spirals_Direction"]);
                settings.erase("E_SLIDER_Spirals_Direction");
                if (!ftt) {
                    int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                    int totalTime = endMS - startMS;
                    int maxState = totalTime * speed / 50;
                    cycles = maxState / 60.0;
                }
                settings["E_SLIDER_Spirals_Movement"] = wxString::Format("%d", dir * cycles);
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_CURTAIN:
            if (settings.Get("E_SLIDER_Curtain_Speed", "") == "") {
                int cycles = 10;
                if (!ftt) {
                    int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                    int totalTime = endMS - startMS;
                    int maxState = totalTime * speed / 50;
                    cycles = maxState / 20.0;
                }
                settings["E_SLIDER_Curtain_Speed"] = wxString::Format("%d", cycles);
            }
            break;
        //these all need code updated and new sliders and such before we can map them
            //these have fitToTime requirements
        case BitmapCache::RGB_EFFECTS_e::eff_SINGLESTRAND:
            if ("Skips" == settings["E_NOTEBOOK_SSEFFECT_TYPE"]) {
                if (settings.Get("E_SLIDER_Skips_Advance", "") == "") {
                    int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                    settings["E_SLIDER_Skips_Advance"] = wxString::Format("%d", speed - 1);
                }
            } else {
                if (settings.Get("E_SLIDER_Chase_Rotations", "") == "") {
                    int cycles = 10;
                    if (!ftt) {
                        int speed = wxAtoi(settings.Get("T_SLIDER_Speed", "10"));
                        int totalTime = endMS - startMS;
                        int maxState = totalTime * speed / 50;
                        cycles = maxState / 25.0;
                    }
                    settings["E_SLIDER_Chase_Rotations"] = wxString::Format("%d", cycles);
                }
            }
            break;
        case BitmapCache::RGB_EFFECTS_e::eff_PICTURES:
            //these all have state/speed requirements
        case BitmapCache::RGB_EFFECTS_e::eff_TEXT:
        case BitmapCache::RGB_EFFECTS_e::eff_GARLANDS:
        case BitmapCache::RGB_EFFECTS_e::eff_METEORS:
        case BitmapCache::RGB_EFFECTS_e::eff_PIANO:
        case BitmapCache::RGB_EFFECTS_e::eff_PINWHEEL:
        case BitmapCache::RGB_EFFECTS_e::eff_SHIMMER:
        case BitmapCache::RGB_EFFECTS_e::eff_SNOWFLAKES:
        case BitmapCache::RGB_EFFECTS_e::eff_SNOWSTORM:
        case BitmapCache::RGB_EFFECTS_e::eff_STROBE:
        case BitmapCache::RGB_EFFECTS_e::eff_TREE:
        case BitmapCache::RGB_EFFECTS_e::eff_TWINKLE:
        case BitmapCache::RGB_EFFECTS_e::eff_WAVE:
            break;
    }
}

Effect::Effect(EffectLayer* parent,int id, int effectIndex, const wxString &name, const wxString &settings, const wxString &palette,
       double startTime,double endTime, int Selected, bool Protected)
    : mParentLayer(parent), mID(id), mEffectIndex(effectIndex), mName(name),
      mStartTime(startTime), mEndTime(endTime), mSelected(Selected), mProtected(Protected),
    changeCount(0)
{
    mSettings.Parse(settings);
    AdjustSettingsToBeFitToTime(mEffectIndex, mSettings, mStartTime * 1000.0, mEndTime * 1000.0);
    mPaletteMap.Parse(palette);
    mColors.clear();
    if (!mPaletteMap.empty()) {
        for (int i = 1; i <= 6; i++) {
            if (mPaletteMap[wxString::Format("C_CHECKBOX_Palette%d",i)] ==  "1") {
                mColors.push_back(xlColor(mPaletteMap[wxString::Format("C_BUTTON_Palette%d",i)]));
            }
        }
    }
}


Effect::~Effect()
{
}

int Effect::GetID() const
{
    return mID;
}

void Effect::SetID(int id)
{
    mID = id;
}


void Effect::SetSettings(const wxString &settings)
{
    mSettings.Parse(settings);
    IncrementChangeCount();
    mDirty = true;
}

void Effect::SetPalette(const wxString& i)
{
    mPaletteMap.Parse(i);
    mColors.clear();
    IncrementChangeCount();
    mDirty = true;
    if (mPaletteMap.empty()) {
        return;
    }
    for (int i = 1; i <= 6; i++) {
        if (mPaletteMap[wxString::Format("C_CHECKBOX_Palette%d",i)] ==  "1") {
            mColors.push_back(xlColor(mPaletteMap[wxString::Format("C_BUTTON_Palette%d",i)]));
        }
    }
}


wxString Effect::GetEffectName() const
{
    return mName;
}

void Effect::SetEffectName(const wxString & name)
{
    mName = name;
    IncrementChangeCount();
}

int Effect::GetEffectIndex() const
{
    return mEffectIndex;
}

void Effect::SetEffectIndex(int effectIndex)
{
    mEffectIndex = effectIndex;
    mDirty = true;
}


double Effect::GetStartTime() const
{
    return mStartTime;
}

void Effect::SetStartTime(double startTime)
{
    if (startTime > mStartTime) {
        IncrementChangeCount();
        mStartTime = startTime;
    } else {
        mStartTime = startTime;
        IncrementChangeCount();
    }
    mDirty = true;
}

double Effect::GetEndTime() const
{
    return mEndTime;
}

void Effect::SetEndTime(double endTime)
{
    if (endTime < mEndTime) {
        IncrementChangeCount();
        mEndTime = endTime;
    } else {
        mEndTime = endTime;
        IncrementChangeCount();
    }
    mDirty = true;
}


int Effect::GetSelected()
{
    return mSelected;
}

void Effect::SetSelected(int selected)
{
    mSelected = selected;
}

bool Effect::GetProtected()
{
    return mProtected;
}
void Effect::SetProtected(bool Protected)
{
    mProtected = Protected;
}

int Effect::GetStartPosition()
{
    return mStartPosition;
}

void Effect::SetStartPosition(int position)
{
    mStartPosition = position;
}

int Effect::GetEndPosition()
{
    return mEndPosition;
}

void Effect::SetEndPosition(int position)
{
    mEndPosition = position;
}

bool Effect::IsDirty()
{
    return mDirty;
}

void Effect::SetDirty(bool dirty)
{
    mDirty = dirty;
}

bool operator<(const Effect &e1, const Effect &e2){
    if(e1.GetStartTime() < e2.GetStartTime())
        return true;
    else
        return false;
}

int Effect::GetEffectIndex(wxString effectName)
{
    if(effectName=="Off"){return BitmapCache::RGB_EFFECTS_e::eff_OFF;}
    else if(effectName=="On"){return BitmapCache::RGB_EFFECTS_e::eff_ON;}
    else if(effectName=="Bars"){return BitmapCache::RGB_EFFECTS_e::eff_BARS;}
    else if(effectName=="Butterfly"){return BitmapCache::RGB_EFFECTS_e::eff_BUTTERFLY;}
    else if(effectName=="Circles"){return BitmapCache::RGB_EFFECTS_e::eff_CIRCLES;}
    else if(effectName=="Color Wash"){return BitmapCache::RGB_EFFECTS_e::eff_COLORWASH;}
    else if(effectName=="Curtain"){return BitmapCache::RGB_EFFECTS_e::eff_CURTAIN;}
    else if(effectName=="Faces"){return BitmapCache::RGB_EFFECTS_e::eff_FACES;}
    else if(effectName=="Fan"){return BitmapCache::RGB_EFFECTS_e::eff_FAN;}
    else if(effectName=="Fire"){return BitmapCache::RGB_EFFECTS_e::eff_FIRE;}
    else if(effectName=="Fireworks"){return BitmapCache::RGB_EFFECTS_e::eff_FIREWORKS;}
    else if(effectName=="Galaxy"){return BitmapCache::RGB_EFFECTS_e::eff_GALAXY;}
    else if(effectName=="Garlands"){return BitmapCache::RGB_EFFECTS_e::eff_GARLANDS;}
    else if(effectName=="Glediator"){return BitmapCache::RGB_EFFECTS_e::eff_GLEDIATOR;}
    else if(effectName=="Life"){return BitmapCache::RGB_EFFECTS_e::eff_LIFE;}
    else if(effectName=="Meteors"){return BitmapCache::RGB_EFFECTS_e::eff_METEORS;}
    else if(effectName=="Morph"){return BitmapCache::RGB_EFFECTS_e::eff_MORPH;}
    else if(effectName=="Piano"){return BitmapCache::RGB_EFFECTS_e::eff_PIANO;}
    else if(effectName=="Pictures"){return BitmapCache::RGB_EFFECTS_e::eff_PICTURES;}
    else if(effectName=="Pinwheel"){return BitmapCache::RGB_EFFECTS_e::eff_PINWHEEL;}
    else if(effectName=="Ripple"){return BitmapCache::RGB_EFFECTS_e::eff_RIPPLE;}
    else if(effectName=="Shimmer"){return BitmapCache::RGB_EFFECTS_e::eff_SHIMMER;}
    else if(effectName=="Shockwave"){return BitmapCache::RGB_EFFECTS_e::eff_SHOCKWAVE;}
    else if(effectName=="SingleStrand"){return BitmapCache::RGB_EFFECTS_e::eff_SINGLESTRAND;}
    else if(effectName=="Snowflakes"){return BitmapCache::RGB_EFFECTS_e::eff_SNOWFLAKES;}
    else if(effectName=="Snowstorm"){return BitmapCache::RGB_EFFECTS_e::eff_SNOWSTORM;}
    else if(effectName=="Spirals"){return BitmapCache::RGB_EFFECTS_e::eff_SPIRALS;}
    else if(effectName=="Spirograph"){return BitmapCache::RGB_EFFECTS_e::eff_SPIROGRAPH;}
    else if(effectName=="Strobe"){return BitmapCache::RGB_EFFECTS_e::eff_STROBE;}
    else if(effectName=="Text"){return BitmapCache::RGB_EFFECTS_e::eff_TEXT;}
    else if(effectName=="Tree"){return BitmapCache::RGB_EFFECTS_e::eff_TREE;}
    else if(effectName=="Twinkle"){return BitmapCache::RGB_EFFECTS_e::eff_TWINKLE;}
    else if(effectName=="Wave"){return BitmapCache::RGB_EFFECTS_e::eff_WAVE;}
    else{return BitmapCache::RGB_EFFECTS_e::eff_OFF;}
}

EffectLayer* Effect::GetParentEffectLayer()
{
    return mParentLayer;
}

void Effect::SetParentEffectLayer(EffectLayer* parent)
{
    mParentLayer = parent;
}

void Effect::IncrementChangeCount()
{
    mParentLayer->IncrementChangeCount(GetStartTime() * 1000, GetEndTime() * 1000);
    changeCount++;
}


