{
    "patcher": {
        "fileversion": 1,
        "appversion": {
            "major": 9,
            "minor": 2,
            "revision": 0,
            "architecture": "x64",
            "modernui": 1
        },
        "classnamespace": "box",
        "rect": [ 1935.0, -299.0, 1953.0, 1225.0 ],
        "boxes": [
            {
                "box": {
                    "fontsize": 30.0,
                    "id": "obj-26",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.333333333333332, 909.3333333333334, 285.6666666666667, 40.0 ],
                    "text": "Tips:"
                }
            },
            {
                "box": {
                    "id": "obj-87",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 172.0, 661.999999821186, 251.33333333333334, 20.0 ],
                    "text": "The overdrive affects all three audio features"
                }
            },
            {
                "box": {
                    "fontname": "Arial",
                    "fontsize": 13.0,
                    "id": "obj-13",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 25.333333333333332, 722.0, 46.0, 23.0 ],
                    "text": "*~ 0.2"
                }
            },
            {
                "box": {
                    "id": "obj-84",
                    "maxclass": "slider",
                    "min": 1.0,
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "parameter_enable": 0,
                    "patching_rect": [ 71.33333333333333, 665.999999731779, 94.40000140666962, 12.000000178813934 ],
                    "size": 2000.0
                }
            },
            {
                "box": {
                    "id": "obj-80",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 255.33333333333334, 868.0, 115.20000171661377, 20.0 ],
                    "text": "Crest Factor"
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-81",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 255.33333333333334, 842.6666666666666, 56.0, 22.0 ],
                    "sig": 0.0
                }
            },
            {
                "box": {
                    "channels": 1,
                    "id": "obj-77",
                    "lastchannelcount": 0,
                    "maxclass": "live.gain~",
                    "numinlets": 1,
                    "numoutlets": 4,
                    "orientation": 1,
                    "outlettype": [ "signal", "", "float", "list" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 25.333333333333332, 610.6666666666665, 152.0, 41.0 ],
                    "saved_attribute_attributes": {
                        "valueof": {
                            "parameter_longname": "live.gain~",
                            "parameter_mmax": 6.0,
                            "parameter_mmin": -70.0,
                            "parameter_modmode": 3,
                            "parameter_shortname": "gain",
                            "parameter_type": 0,
                            "parameter_unitstyle": 4
                        }
                    },
                    "varname": "live.gain~"
                }
            },
            {
                "box": {
                    "id": "obj-73",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 181.33333333333334, 610.6666666666665, 219.20000326633453, 33.0 ],
                    "text": "The gain moves the RMS, and drives the overdrive"
                }
            },
            {
                "box": {
                    "id": "obj-72",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 25.333333333333325, 687.0000000000001, 65.0, 22.0 ],
                    "text": "overdrive~"
                }
            },
            {
                "box": {
                    "id": "obj-67",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 81.99999922513962, 525.9999999999999, 219.20000326633453, 33.0 ],
                    "text": "Change the frequency of the oscillator to affect the spectral centroid"
                }
            },
            {
                "box": {
                    "id": "obj-66",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 134.0, 868.0, 115.20000171661377, 20.0 ],
                    "text": "Root Mean Square"
                }
            },
            {
                "box": {
                    "id": "obj-65",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.333333333333332, 868.0, 102.4000015258789, 20.0 ],
                    "text": "Spectral Centroid"
                }
            },
            {
                "box": {
                    "id": "obj-62",
                    "maxclass": "number",
                    "maximum": 2000,
                    "minimum": 50,
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [ "", "bang" ],
                    "parameter_enable": 0,
                    "patching_rect": [ 25.33333333333333, 531.4999999999999, 50.0, 22.0 ]
                }
            },
            {
                "box": {
                    "fontname": "Arial",
                    "fontsize": 11.595187,
                    "id": "obj-59",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 25.333333333333325, 568.6666666666666, 66.0, 21.0 ],
                    "text": "cycle~ 440"
                }
            },
            {
                "box": {
                    "autofit": 1,
                    "forceaspect": 1,
                    "id": "obj-53",
                    "maxclass": "fpic",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "jit_matrix" ],
                    "patching_rect": [ 531.3333333333334, 34.666666666666664, 103.20000153779984, 103.20000153779984 ],
                    "pic": "zerr-logo-white-transparent.png"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-45",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 409.0, 420.0, 22.0 ],
                    "text": "\"zc\" -----> zero crossings, a pulse of 1 at every crossing"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-46",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 386.3333333333333, 420.0, 22.0 ],
                    "text": "\"cf\" -----> crest factor, peak over RMS, 1 and up"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-47",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 363.6666666666667, 420.0, 22.0 ],
                    "text": "\"flt\" ----> spectral flatness, 0 for a tone, 1 for noise"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-48",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 341.0, 420.0, 22.0 ],
                    "text": "\"rlf\" ----> spectral rolloff, in Hz"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-43",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.0, 319.0, 420.0, 22.0 ],
                    "text": "\"ctd\" ----> spectral centroid, brightness, in Hz"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-44",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 296.3333333333333, 420.0, 22.0 ],
                    "text": "\"flx\" ----> spectral flux, how fast the spectrum changes, 0 and up"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-42",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 273.6666666666667, 420.0, 22.0 ],
                    "text": "\"zcr\" ----> zero-crossing rate, noisiness, 0 to 1"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-38",
                    "linecount": 5,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 433.0, 610.2000015377998, 85.0 ],
                    "text": "In the help patch, a 440 Hz sine goes through a gain and [overdrive~] into [zerr.features~ ctd rms cf], which outputs its spectral centroid, RMS and crest factor. Raise the overdrive and the centroid climbs as harmonics appear while the crest factor falls as the wave flattens. The gain moves the RMS directly and, through the overdrive, the other two a little. Change the oscillator frequency and the centroid follows it."
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-24",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 251.0, 420.0, 22.0 ],
                    "text": "\"rms\" ----> root mean square, loudness, 0 to 1"
                }
            },
            {
                "box": {
                    "id": "obj-22",
                    "maxclass": "live.line",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.0, 162.66666666666666, 609.5333348711332, 8.0 ]
                }
            },
            {
                "box": {
                    "fontsize": 64.0,
                    "id": "obj-17",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.0, 34.666666666666664, 404.0, 78.0 ],
                    "text": "zerr.features~"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-15",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 227.66666666666666, 173.0, 22.0 ],
                    "text": "Available features:"
                }
            },
            {
                "box": {
                    "fontsize": 18.0,
                    "id": "obj-14",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.0, 127.33333333333333, 532.0, 27.0 ],
                    "text": "- extract audio features from a signal as audio-rate control signals"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-11",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 24.333333333333336, 172.66666666666666, 610.2000015377998, 53.0 ],
                    "text": "The [zerr.features~] extracts audio features from a mono signal and outputs each one as an audio-rate signal. Every argument names a feature and adds an outlet, in argument order; with no argument it extracts rms. The outlets are the control signals that drive [zerr.envelopes~]."
                }
            },
            {
                "box": {
                    "id": "obj-4",
                    "local": 1,
                    "maxclass": "ezdac~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 589.5333348711333, 520.0, 45.0, 45.0 ],
                    "varname": "autohelp_dac"
                }
            },
            {
                "box": {
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-34",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 548.8666682044667, 526.6666666666666, 40.0, 33.0 ],
                    "text": "start audio",
                    "varname": "autohelp_dac_text"
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-2",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 134.0, 842.6666666666666, 56.0, 22.0 ],
                    "sig": 0.0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-1",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 25.333333333333332, 842.6666666666666, 56.0, 22.0 ],
                    "sig": 0.0
                }
            },
            {
                "box": {
                    "id": "obj-20",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 3,
                    "outlettype": [ "signal", "signal", "signal" ],
                    "patching_rect": [ 25.333333333333332, 781.3333333333334, 138.0, 22.0 ],
                    "text": "zerr.features~ ctd rms cf"
                }
            },
            {
                "box": {
                    "id": "obj-88",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 149.99999999999997, 722.4999999999999, 40.0, 22.0 ],
                    "text": "bang"
                }
            },
            {
                "box": {
                    "id": "obj-89",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 193.33333333333334, 723.4999999999999, 420.0, 20.0 ],
                    "text": "bang: post version, the features in outlet order, block size and sample rate"
                }
            },
            {
                "box": {
                    "id": "obj-91",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 172.0, 768.8333333333334, 441.33333333333337, 47.0 ],
                    "text": "Warning: the features are not normalised. They are control signals with very different ranges (ctd and rlf in Hz, cf and flx above 1). Do not connect an outlet to [dac~] or [ezdac~]; scale it and send it to [zerr.envelopes~].",
                    "textcolor": [ 0.796078431372549, 0.054901960784313725, 0.054901960784313725, 1.0 ]
                }
            },
            {
                "box": {
                    "id": "obj-93",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.333333333333332, 955.3333333333334, 618.6666666666666, 33.0 ],
                    "text": "- One value per signal vector. All features except zc are measured over the last 2048 samples and ramped across each vector, so they are smooth and lag by about 46 ms at 44.1 kHz. A larger vector size makes them coarser."
                }
            },
            {
                "box": {
                    "id": "obj-94",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.333333333333332, 990.3333333333334, 618.6666666666666, 33.0 ],
                    "text": "- Scale before [zerr.envelopes~]. Its control inlet wants 0 to 1 and the ranges differ: divide ctd or rlf by a top frequency and [clip~ 0 1], or use [scale~]. rms, zcr and flt can go in as they are."
                }
            },
            {
                "box": {
                    "id": "obj-95",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.333333333333332, 1025.3333333333335, 618.6666666666666, 33.0 ],
                    "text": "- zc is a ready trigger. It touches 1 exactly on every zero crossing, so it drives a trigger-mode [zerr.envelopes~] directly. Set interval there, or the sound jumps hundreds of times a second (which sounds amazing:)."
                }
            },
            {
                "box": {
                    "id": "obj-96",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 25.333333333333332, 1060.3333333333335, 618.6666666666666, 33.0 ],
                    "text": "- Level leaks into rms and flx only. ctd, rlf, flt, cf and zcr are ratios or frequencies and ignore the gain. Normalise the input if loudness should not shape the distribution."
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [ "obj-20", 0 ],
                    "source": [ "obj-13", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-1", 0 ],
                    "source": [ "obj-20", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-2", 0 ],
                    "midpoints": [ 94.33333333333333, 822.7682398358982, 143.5, 822.7682398358982 ],
                    "source": [ "obj-20", 1 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-81", 0 ],
                    "midpoints": [ 153.83333333333334, 826.6562606692314, 264.83333333333337, 826.6562606692314 ],
                    "source": [ "obj-20", 2 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-77", 0 ],
                    "source": [ "obj-59", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-59", 0 ],
                    "source": [ "obj-62", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-13", 0 ],
                    "source": [ "obj-72", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-72", 0 ],
                    "source": [ "obj-77", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-72", 1 ],
                    "source": [ "obj-84", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-20", 0 ],
                    "midpoints": [ 159.49999999999997, 757.4479166666666, 34.83333333333333, 757.4479166666666 ],
                    "source": [ "obj-88", 0 ]
                }
            }
        ],
        "parameters": {
            "obj-77": [ "live.gain~", "gain", 0 ],
            "parameterbanks": {
                "0": {
                    "index": 0,
                    "name": "",
                    "parameters": [ "-", "-", "-", "-", "-", "-", "-", "-" ],
                    "buttons": [ "-", "-", "-", "-", "-", "-", "-", "-" ]
                }
            },
            "inherited_shortname": 1
        },
        "autosave": 0
    }
}