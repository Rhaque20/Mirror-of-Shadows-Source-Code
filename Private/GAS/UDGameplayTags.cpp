#include "GAS/UDGameplayTags.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Status_Daze, "Damage.Status.Daze");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Status_Burn, "Damage.Status.Burn");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Status_Silence, "Damage.Status.Silence");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Status_Freeze, "Damage.Status.Freeze");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Status_Slow, "Damage.Status.Slow");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Status_Shock, "Damage.Status.Shock");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Status_Curse, "Damage.Status.Curse");

UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Stagger_Poise, "Damage.Stagger.Poise");

UE_DEFINE_GAMEPLAY_TAG(TAG_Skill, "Skill")

// Health damage
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health,"Damage.Health");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health_Fire,"Damage.Health.Fire");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health_Wind,"Damage.Health.Wind");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health_Earth,"Damage.Health.Earth");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health_Water,"Damage.Health.Water");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health_Light,"Damage.Health.Light");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health_Dark,"Damage.Health.Dark");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Health_Physical,"Damage.Health.Physical");

// Status Ailments
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Ailment_Daze, "Status.Ailment.Daze");
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Ailment_Burn, "Status.Ailment.Burn");
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Ailment_Silence, "Status.Ailment.Silence");
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Ailment_Slow, "Status.Ailment.Slow");
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Ailment_Freeze, "Status.Ailment.Freeze")
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Ailment_Shock, "Status.Ailment.Shock")
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Ailment_Curse, "Status.Ailment.Curse")

// Debuffs
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Debuff_Control_Bind,"Status.Debuff.Control.Bind")

UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_PoiseBroken, "Effect.State.PoiseBroken")
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_Stagger,"Effect.State.Stagger")
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_Stagger_Stunned,"Effect.State.Stagger.Stun")
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_Stagger_KnockedDown, "Effect.State.Stagger.KnockedDown")
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_Stagger_StaggerFall, "Effect.State.Stagger.StaggerFall")


UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Defensive_Block, "Effect.Defensive.Block");

// Combat states
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_OffField, "Effect.State.OffField")
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_Airborne, "Effect.State.Airborne")
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_OnGround, "Effect.State.OnGround")
UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_Dead, "Effect.State.Dead")

UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_State_AttackDelay, "Effect.State.AttackDelay")

//For combat tickets
UE_DEFINE_GAMEPLAY_TAG(TAG_EnemyAI_Ticket_Melee, "EnemyAI.Ticket.Melee")
UE_DEFINE_GAMEPLAY_TAG(TAG_EnemyAI_Ticket_Range, "EnemyAI.Ticket.Range")

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Input_NormalAttack, "Ability.Input.NormalAttack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Input_Skill, "Ability.Input.Skill")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Input_Skill_Activate, "Ability.Input.Skill.Activate")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Input_Jump, "Ability.Input.Jump")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Input, "Ability.Input")

UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Flat_HP, "Stats.Flat.HP")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Flat_ATK,"Stats.Flat.ATK")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Flat_DEF, "Stats.Flat.DEF")

UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_HP, "Stats.Percent.HP")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_ATK, "Stats.Percent.ATK")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_DEF, "Stats.Percent.DEF")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_Potency, "Stats.Percent.Potency")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_Resistance, "Stats.Percent.Resistance")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_CritRate, "Stats.Percent.CritRate")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_CritDMG, "Stats.Percent.CritDMG")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_ChargeRate, "Stats.Percent.ChargeRate")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_SPGain, "Stats.Percent.SPGain")

UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_LiteATKDMGUp,"Stats.Percent.LiteATKDMGUp")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_ComboATKDMGUp,"Stats.Percent.ComboATKDMGUp")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_HeavyATKDMGUp,"Stats.Percent.HeavyATKDMGUp")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_StunDMGUp,"Stats.Percent.StunDMGUp")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_SkillChainBoost,"Stats.Percent.SkillChainBoost")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_HealBonus,"Stats.Percent.HealBonus")
UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_Percent_Tenacity,"Stats.Percent.Tenacity")

// Equipment
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Armor_Helmet,"Equipment.Armor.Helmet")
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Armor_Chestplate,"Equipment.Armor.Chestplate")
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Armor_Boots,"Equipment.Armor.Boots")
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Armor_Bracer,"Equipment.Armor.Bracer")
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Armor_Necklace,"Equipment.Armor.Necklace")
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Armor_Ring,"Equipment.Armor.Ring")

// Armor Sets
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Set_ScoutArmor,"Equipment.Set.ScoutArmor")
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Set_BeastHunter,"Equipment.Set.BeastHunter")
UE_DEFINE_GAMEPLAY_TAG(TAG_Equipment_Set_BrightStar,"Equipment.Set.BrightStar")

// Combat Cooldowns
UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Charge_Evade, "Skill.Charge.Evade")
UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Charge_Block, "Skill.Charge.Block")

// Combat Events
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Combat_SkillActivate, "Event.Combat.SkillActivate")
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Combat_Hit, "Event.Combat.Hit")
// Used for when getting healed
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Combat_Healed, "Event.Combat.Healed")
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Combat_ProjectileHit, "Event.Combat.ProjectileHit")
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Combat_HitConnect, "Event.Combat.HitConnect")