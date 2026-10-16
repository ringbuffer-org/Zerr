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
        "rect": [ 2417.0, -238.0, 1087.0, 822.0 ],
        "boxes": [
            {
                "box": {
                    "id": "obj-40",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 4,
                    "outlettype": [ "signal", "signal", "signal", "signal" ],
                    "patching_rect": [ 483.0, 437.6, 84.0, 22.0 ],
                    "text": "mc.unpack~ 4"
                }
            },
            {
                "box": {
                    "id": "obj-39",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 483.0, 403.2, 107.0, 22.0 ],
                    "text": "mc.r~ envelopes 4"
                }
            },
            {
                "box": {
                    "id": "obj-38",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 156.24999999999997, 701.3392857142857, 99.0, 22.0 ],
                    "text": "mc.s~ envelopes"
                }
            },
            {
                "box": {
                    "bubble": 1,
                    "id": "obj-31",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 57.000000000000014, 794.0, 320.0, 64.0 ],
                    "text": "Channel count follows the envelopes, not the layout file. Swap the layout argument of [zerr.envelopes~] and [zerr.disperser~] adapts on its own; nothing needs to be set here."
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-21",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 483.0, 776.0, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-24",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 483.0, 683.2, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-18",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 483.0, 587.2, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "bubble": 1,
                    "id": "obj-17",
                    "linecount": 6,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 204.0178571428571, 593.3035714285713, 247.7678571428571, 91.0 ],
                    "text": "Envelopes are gains and are not limited. [zerr.envelopes~] stays within 0 to 1, but if you build envelopes yourself, values above 1 boost and negative values flip the phase. Keep them in 0 to 1 unless that is what you want."
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-58",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 483.0, 493.6, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-11",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 826.0, 776.0, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-12",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 826.0, 683.2, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-10",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 826.0, 587.2, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "displaychan": 4,
                    "id": "obj-7",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 826.0, 493.6, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "id": "obj-62",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 301.5, 424.0, 44.0, 22.0 ],
                    "text": "noise~"
                }
            },
            {
                "box": {
                    "id": "obj-60",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 215.0, 458.4, 43.0, 22.0 ],
                    "text": ">~ 0.5"
                }
            },
            {
                "box": {
                    "id": "obj-61",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 215.0, 424.0, 72.0, 22.0 ],
                    "text": "phasor~ 0.5"
                }
            },
            {
                "box": {
                    "bufsize": 256,
                    "calccount": 16,
                    "id": "obj-57",
                    "maxclass": "scope~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 129.6, 326.4, 315.0, 78.0 ],
                    "style": "default"
                }
            },
            {
                "box": {
                    "id": "obj-55",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 129.6, 458.4, 55.0, 22.0 ],
                    "text": "triangle~"
                }
            },
            {
                "box": {
                    "id": "obj-54",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 129.6, 424.0, 72.0, 22.0 ],
                    "text": "phasor~ 0.5"
                }
            },
            {
                "box": {
                    "id": "obj-49",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.599999999999994, 870.0, 84.0, 22.0 ],
                    "text": "mc.s~ outputs"
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-47",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 363.20000000000005, 424.0, 56.0, 22.0 ],
                    "sig": 0.4
                }
            },
            {
                "box": {
                    "id": "obj-35",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 129.6, 288.0, 59.0, 22.0 ],
                    "text": "r~ source"
                }
            },
            {
                "box": {
                    "id": "obj-33",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 57.000000000000014, 326.4, 61.0, 22.0 ],
                    "text": "s~ source"
                }
            },
            {
                "box": {
                    "id": "obj-29",
                    "local": 1,
                    "maxclass": "ezdac~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 671.0, 288.0, 45.0, 45.0 ],
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
                    "patching_rect": [ 629.0, 288.0, 40.0, 33.0 ],
                    "text": "start audio",
                    "varname": "autohelp_dac_text"
                }
            },
            {
                "box": {
                    "id": "obj-28",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 40.000000000000014, 288.0, 66.0, 22.0 ],
                    "text": "cycle~ 100"
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
                    "patching_rect": [ 612.7999984622002, 16.79999846220018, 103.20000153779984, 103.20000153779984 ],
                    "pic": "zerr-logo-white-transparent.png"
                }
            },
            {
                "box": {
                    "id": "obj-19",
                    "justification": 1,
                    "maxclass": "live.line",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 151.0, 676.0, 8.400002181529999 ]
                }
            },
            {
                "box": {
                    "fontsize": 64.0,
                    "id": "obj-20",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 23.399999231100082, 567.0, 78.0 ],
                    "text": "zerr.disperser~"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-22",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 216.40000218153, 676.0, 69.0 ],
                    "text": "In the help patch, the source is a 100 Hz sine from [cycle~]. Four envelopes are packed with [mc.pack~ 4]: a slow triangle wave, a square pulse, white noise and a [number~] you set by hand. Every output channel is the same sine, scaled by its own envelope, so the four channels behave differently from one tone. You can check the waveforms of the envelopes and the waveforms of the outputs for an intuitive feeling"
                }
            },
            {
                "box": {
                    "fontsize": 18.0,
                    "id": "obj-23",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 122.0, 702.4, 27.0 ],
                    "text": "- disperse a mono source over the speakers, each channel shaped by its envelope"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-25",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 161.40000218153, 676.0, 53.0 ],
                    "text": "The [zerr.disperser~] multiplies the source signal with the envelopes to achieve multichannel panning. The first inlet receives the mono source signal; the second inlet receives the multichannel envelopes. The outlet carries one channel per envelope."
                }
            },
            {
                "box": {
                    "id": "obj-8",
                    "maxclass": "newobj",
                    "numinlets": 4,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 129.6, 627.8035714285713, 70.0, 22.0 ],
                    "text": "mc.pack~ 4"
                }
            },
            {
                "box": {
                    "id": "obj-1",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 40.599999999999994, 752.0, 107.99999999999999, 22.0 ],
                    "text": "zerr.disperser~"
                }
            },
            {
                "box": {
                    "bubble": 1,
                    "id": "obj-63",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 156.6, 737.0, 259.2, 51.0 ],
                    "text": "The source inlet must be mono: a multichannel source silences the output. Sum or pick a channel first."
                }
            },
            {
                "box": {
                    "id": "obj-50",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 826.0, 403.2, 92.0, 22.0 ],
                    "text": "mc.r~ outputs 4"
                }
            },
            {
                "box": {
                    "id": "obj-9",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 4,
                    "outlettype": [ "signal", "signal", "signal", "signal" ],
                    "patching_rect": [ 826.0, 437.6, 84.0, 22.0 ],
                    "text": "mc.unpack~ 4"
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [ "obj-49", 0 ],
                    "source": [ "obj-1", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-1", 0 ],
                    "order": 1,
                    "source": [ "obj-28", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-33", 0 ],
                    "order": 0,
                    "source": [ "obj-28", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-57", 0 ],
                    "source": [ "obj-35", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-40", 0 ],
                    "source": [ "obj-39", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-18", 0 ],
                    "midpoints": [ 514.1666666666667, 479.0, 469.0, 479.0, 469.0, 583.9999999999998, 492.5, 583.9999999999998 ],
                    "source": [ "obj-40", 1 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-21", 0 ],
                    "midpoints": [ 557.5, 479.0, 469.0, 479.0, 469.0, 772.9999999999998, 492.5, 772.9999999999998 ],
                    "source": [ "obj-40", 3 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-24", 0 ],
                    "midpoints": [ 535.8333333333334, 479.0, 469.0, 479.0, 469.0, 679.9999999999998, 492.5, 679.9999999999998 ],
                    "source": [ "obj-40", 2 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-58", 0 ],
                    "source": [ "obj-40", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-8", 3 ],
                    "midpoints": [ 372.70000000000005, 583.1265624999999, 190.1, 583.1265624999999 ],
                    "source": [ "obj-47", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-9", 0 ],
                    "source": [ "obj-50", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-55", 0 ],
                    "source": [ "obj-54", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-8", 0 ],
                    "source": [ "obj-55", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-8", 1 ],
                    "midpoints": [ 224.5, 509.9710937499999, 156.1, 509.9710937499999 ],
                    "source": [ "obj-60", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-60", 0 ],
                    "source": [ "obj-61", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-8", 2 ],
                    "midpoints": [ 311.0, 545.8296874999999, 173.1, 545.8296874999999 ],
                    "source": [ "obj-62", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-1", 1 ],
                    "order": 1,
                    "source": [ "obj-8", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-38", 0 ],
                    "order": 0,
                    "source": [ "obj-8", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-10", 0 ],
                    "midpoints": [ 857.1666666666666, 479.0, 813.0, 479.0, 813.0, 583.9999999999998, 835.5, 583.9999999999998 ],
                    "source": [ "obj-9", 1 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-11", 0 ],
                    "midpoints": [ 900.5, 479.0, 813.0, 479.0, 813.0, 772.9999999999998, 835.5, 772.9999999999998 ],
                    "source": [ "obj-9", 3 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-12", 0 ],
                    "midpoints": [ 878.8333333333334, 479.0, 813.0, 479.0, 813.0, 679.9999999999998, 835.5, 679.9999999999998 ],
                    "source": [ "obj-9", 2 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-7", 0 ],
                    "source": [ "obj-9", 0 ]
                }
            }
        ],
        "autosave": 0,
        "styles": [
            {
                "name": "rnbodefault",
                "default": {
                    "accentcolor": [ 0.343034118413925, 0.506230533123016, 0.86220508813858, 1.0 ],
                    "bgcolor": [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
                    "bgfillcolor": {
                        "angle": 270.0,
                        "autogradient": 0.0,
                        "color": [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
                        "color1": [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
                        "color2": [ 0.263682, 0.004541, 0.038797, 1.0 ],
                        "proportion": 0.39,
                        "type": "color"
                    },
                    "color": [ 0.929412, 0.929412, 0.352941, 1.0 ],
                    "elementcolor": [ 0.357540726661682, 0.515565991401672, 0.861786782741547, 1.0 ],
                    "fontname": [ "Lato" ],
                    "fontsize": [ 12.0 ],
                    "stripecolor": [ 0.258338063955307, 0.352425158023834, 0.511919498443604, 1.0 ],
                    "textcolor_inverse": [ 0.968627, 0.968627, 0.968627, 1 ]
                },
                "parentstyle": "",
                "multi": 0
            },
            {
                "name": "rnbohighcontrast",
                "default": {
                    "accentcolor": [ 0.666666666666667, 0.666666666666667, 0.666666666666667, 1.0 ],
                    "bgcolor": [ 0.0, 0.0, 0.0, 1.0 ],
                    "bgfillcolor": {
                        "angle": 270.0,
                        "autogradient": 0.0,
                        "color": [ 0.0, 0.0, 0.0, 1.0 ],
                        "color1": [ 0.090196078431373, 0.090196078431373, 0.090196078431373, 1.0 ],
                        "color2": [ 0.156862745098039, 0.168627450980392, 0.164705882352941, 1.0 ],
                        "proportion": 0.5,
                        "type": "color"
                    },
                    "clearcolor": [ 1.0, 1.0, 1.0, 0.0 ],
                    "color": [ 1.0, 0.874509803921569, 0.141176470588235, 1.0 ],
                    "editing_bgcolor": [ 0.258823529411765, 0.258823529411765, 0.258823529411765, 1.0 ],
                    "elementcolor": [ 0.223386004567146, 0.254748553037643, 0.998085916042328, 1.0 ],
                    "fontsize": [ 13.0 ],
                    "locked_bgcolor": [ 0.258823529411765, 0.258823529411765, 0.258823529411765, 1.0 ],
                    "selectioncolor": [ 0.301960784313725, 0.694117647058824, 0.949019607843137, 1.0 ],
                    "stripecolor": [ 0.258823529411765, 0.258823529411765, 0.258823529411765, 1.0 ],
                    "textcolor": [ 1.0, 1.0, 1.0, 1.0 ],
                    "textcolor_inverse": [ 1.0, 1.0, 1.0, 1.0 ]
                },
                "parentstyle": "",
                "multi": 0
            }
        ]
    }
}