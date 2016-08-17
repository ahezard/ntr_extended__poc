cp ntr_extended__poc.nds ntr_extended__poc-ds.nds
cp ntr_extended__poc.nds ntr_extended__poc-dsi.nds
cp ntr_extended__poc.nds ntr_extended__poc-dsinogba.nds

python patch_ndsheader_dsiware.py  --title "TEST CPU SPEED" --maker 01 --code AFFE ntr_extended__poc-ds.nds
python patch_ndsheader_dsiware.py  --mode dsi --title "TEST CPU SPEED" --maker 01 --code KPOE ntr_extended__poc-dsi.nds
python patch_ndsheader_dsiware.py  --mode dsinogba --title "TEST CPU SPEED" --maker 01 --code KPOE ntr_extended__poc-dsinogba.nds

./make_cia --srl=ntr_extended__poc-ds.nds
./make_cia --srl=ntr_extended__poc-dsi.nds