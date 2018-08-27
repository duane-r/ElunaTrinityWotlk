#include "ScriptMgr.h"
#include "Pet.h"


// This assumes that every dungeon is five-man, and may not be
//  terribly efficient. (Not an issue for me.)

class custom_make_instances_easier : public PlayerScript
{
  public:
    custom_make_instances_easier() : PlayerScript("custom_make_instances_easier") { }

    void OnDamage(Unit* attacker, Unit* victim, uint32& damage)
    {
        if (!victim || !victim->GetMap()->IsDungeon())
            return;

        uint32 count = victim->GetMap()->GetPlayersCountExceptGMs();
        if (count > 3)
            return;

        uint32 mult = 2;

        if (count == 1)
            mult = 3;

        ObjectGuid vg = victim->GetGUID();
        if (vg.IsPlayer() || (vg.IsPet() && (victim->GetOwnerGUID()).IsPlayer()))
        {
            damage /= mult;
            //printf("Player takes %d damage\n", damage);
            return;
        }

        if (!attacker)
            return;

        ObjectGuid ag = attacker->GetGUID();
        if (ag.IsPlayer() || (ag.IsPet() && (attacker->GetOwnerGUID()).IsPlayer()))
        {
            damage *= mult;
            //printf("Player deals %d damage\n", damage);
            return;
        }
    }


    void OnGiveXP(Player* player, uint32& amount, Unit* victim)
    {
        if (!player || !player->GetMap()->IsDungeon())
            return;

        uint32 count = player->GetMap()->GetPlayersCountExceptGMs();
        if (count > 3)
            return;

        uint32 mult = 2;

        if (count == 1)
            mult = 3;

        //printf("experience lowered from %d to %d\n", amount, amount / 5);
        amount /= mult;
    }
};

void AddSC_custom_make_instances_easier()
{
  new custom_make_instances_easier();
}
